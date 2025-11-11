/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_symbols32.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:16:46 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/11 19:07:30 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include "ft_nm.h"

Elf32_Sym	*get_symbol_table32(t_elf *elf, Elf32_Shdr *shdr)
{
	Elf32_Off const	offset = load_uint32(shdr->sh_offset, elf->swap);
	uint32_t const	size = load_uint32(shdr->sh_size, elf->swap);
	Elf32_Sym		*symtab;

	symtab = (Elf32_Sym *)seek_elf(elf, offset, size);
	if (!symtab)
		return (NULL);
	return (symtab);
}

static void	set_symbol_flags(t_elf *elf, t_symbol *sym)
{
	uint32_t			sec_size;
	Elf32_Shdr const*	shdr = get_section32(elf, \
		load_uint16(sym->u_entry.e32->st_shndx, elf->swap), &sec_size);

	sym->flags = 0;
	if (sym->type == 'N' || sym->type == 'I' || sym->type == 'a')
		sym->flags |= SYM_IS_DBG;
	if (sym->type == 'U' || sym->type == 'w' || sym->type == 'v')
		sym->flags |= SYM_IS_UNDEF;
	if (sym->type == 'T' || sym->type == 'D' || sym->type == 'R' || \
		sym->type == 'B' || sym->type == 'G' || sym->type == 'U' || \
	sym->type == 'W' || sym->type == 'w')
		sym->flags |= SYM_IS_EXT;
	if (!shdr)
		return ;
	if (!(load_uint32(shdr->sh_flags, elf->swap) & SHF_WRITE))
		sym->flags |= SYM_IS_RDONLY;
}

static t_symbol	load_symbol(t_elf *elf, Elf32_Sym *symtab, char *strtab, \
	size_t strtablen)
{
	t_symbol		symbol;
	uint32_t const	name = load_uint32(symtab->st_name, elf->swap);
	uint16_t const	shndx = load_uint16(symtab->st_shndx, elf->swap);

	symbol.u_entry.e32 = symtab;
	symbol.value = load_uint32(symtab->st_value, elf->swap);
	symbol.type = (uint8_t)(get_symbol_type32(elf, symtab) & 0xFF);
	symbol.name = NULL;
	if (ELF32_ST_TYPE(symtab->st_info) == STT_SECTION)
		symbol.name = get_section_name32(elf, shndx);
	else if (strtab && name && name < strtablen && strtab[name] != '\0')
		symbol.name = &strtab[name];
	set_symbol_flags(elf, &symbol);
	return (symbol);
}

static size_t	load_symbols(t_elf *elf, Elf32_Shdr *shdr, t_symbol *symbols)
{
	Elf32_Sym *const	symtab = get_symbol_table32(elf, shdr);
	char				*strtab;
	uint32_t			i;
	uint32_t			symcount;
	uint32_t			strtablen;

	if (!symtab)
		return (0);
	strtablen = 0;
	strtab = (char *)get_section32(elf, load_uint32(shdr->sh_link, elf->swap), \
								&strtablen);
	symcount = load_uint32(shdr->sh_size, elf->swap) / sizeof(Elf32_Sym);
	if (!symcount)
		return (0);
	i = 0;
	while (i < symcount)
	{
		symbols[i] = load_symbol(elf, &symtab[i], strtab, strtablen);
		++i;
	}
	return (i);
}

size_t	load_all_symbols32(t_elf *elf, t_section *sections, t_symbol *symtab, \
	t_symbol *dynsym)
{
	size_t		loaded;
	size_t		i;
	uint16_t	shnum;

	loaded = 0;
	i = 0;
	shnum = load_uint16(elf->u_dat.ehdr32->e_shnum, elf->swap);
	while (i < shnum)
	{
		if (sections[i].type == SHT_SYMTAB)
			loaded += load_symbols(elf, sections[i].u_shdr.e32, symtab);
		else if (sections[i].type == SHT_DYNSYM)
			loaded += load_symbols(elf, sections[i].u_shdr.e32, dynsym);
		++i;
	}
	return (loaded);
}
