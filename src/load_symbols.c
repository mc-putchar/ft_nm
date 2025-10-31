/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 01:55:42 by mcutura           #+#    #+#             */
/*   Updated: 2025/08/10 21:51:03 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include "ft_nm.h"
#include "ft_printf.h"

Elf64_Sym	*get_symbol_table(t_elf *elf, Elf64_Shdr *shdr)
{
	Elf64_Sym	*symtab;

	symtab = (Elf64_Sym *)seek_elf(elf, shdr->sh_offset, shdr->sh_size);
	if (!symtab)
		return (NULL);
	return (symtab);
}

static void	set_symbol_flags(t_elf *elf, t_symbol *sym)
{
	size_t		sec_size;
	Elf64_Shdr	*shdr;

	sym->flags = 0;
	if (sym->type == 'N' || sym->type == 'I' || sym->type == 'a')
		sym->flags |= SYM_IS_DBG;
	if (sym->type == 'U' || sym->type == 'w' || sym->type == 'v')
		sym->flags |= SYM_IS_UNDEF;
	if (sym->type == 'T' || sym->type == 'D' || sym->type == 'R' || \
		sym->type == 'B' || sym->type == 'G' || sym->type == 'U' || \
        sym->type == 'W' || sym->type == 'w')
		sym->flags |= SYM_IS_EXT;
	shdr = get_section(elf, sym->entry->st_shndx, &sec_size);
	if (!shdr)
		return ;
	if (!(shdr->sh_flags & SHF_WRITE))
		sym->flags |= SYM_IS_RDONLY;
}

static t_symbol load_symbol(t_elf *elf, Elf64_Sym *symtab, char *strtab, size_t strtablen)
{
    t_symbol    symbol;

    symbol.entry = symtab;
	symbol.value = symtab->st_value;
	symbol.type = (uint8_t)(get_symbol_type(elf, symtab) & 0xFF);
	symbol.name = NULL;
	if (ELF64_ST_TYPE(symtab->st_info) == STT_SECTION)
		symbol.name = get_section_name(elf, symtab->st_shndx);
	else if (symtab->st_name < strtablen && strtab[symtab->st_name] != '\0')
		symbol.name = &strtab[symtab->st_name];
	set_symbol_flags(elf, &symbol);
	if (DEBUG)
	{
		ft_printf("Symbol: value = %016x, type = %c, ", symbol.value, symbol.type);
		if (symbol.name)
		    ft_printf("name = %s [%u]\n", symbol.name, symtab->st_name);
		ft_printf("  Entry: st_name = %u, st_value = %016x, st_size = %u, st_info = %02x, st_other = %02x, st_shndx = %u\n",
			symtab->st_name, symtab->st_value, symtab->st_size,
			symtab->st_info, symtab->st_other, symtab->st_shndx);
	}
	return (symbol);
}

static size_t	load_symbols(t_elf *elf, Elf64_Shdr *shdr, t_symbol *symbols)
{
	Elf64_Sym *const    symtab = get_symbol_table(elf, shdr);
	size_t			    i;
	size_t			    symcount;
	char			    *strtab;
	size_t			    strtablen;

	if (!symtab)
		return (0);
	strtablen = 0;
	strtab = (char *)get_section(elf, shdr->sh_link, &strtablen);
	symcount = (uint32_t)shdr->sh_size / sizeof(Elf64_Sym);
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

size_t	load_all_symbols(t_elf *elf, t_section *sections, t_symbol *symtab, t_symbol *dynsym)
{
	size_t	loaded;
	size_t	i;

	loaded = 0;
	i = 0;
	while (i < elf->u_dat.ehdr->e_shnum)
	{
		if (sections[i].type == SHT_SYMTAB)
			loaded += load_symbols(elf, sections[i].shdr, symtab);
		else if (sections[i].type == SHT_DYNSYM)
			loaded += load_symbols(elf, sections[i].shdr, dynsym);
		++i;
	}
	return (loaded);
}
