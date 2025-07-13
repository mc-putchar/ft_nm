/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 01:55:42 by mcutura           #+#    #+#             */
/*   Updated: 2025/07/12 23:20:04 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include "ft_nm.h"
#include "ft_printf.h"
#include "libft_str.h"

unsigned char	get_symbol_info(t_elf *elf, Elf64_Sym const *sym)
{
	unsigned char const	info = ELF64_ST_TYPE(sym->st_info);
	unsigned char const	bind = ELF64_ST_BIND(sym->st_info);
	unsigned char		is_local;
	uint32_t			type;
	char				*sec_name;

	if (bind == STB_GLOBAL)
		is_local = 0;
	if (bind == STB_LOCAL)
		is_local = 32;
	if (bind == STB_WEAK)
	{
		if (info == STT_OBJECT)
			return ('V');
		return ('W' + is_local);
	}
	if (bind == STB_GNU_UNIQUE)
		return ('u');
	if (sym->st_shndx == SHN_UNDEF)
		return ('U');
	if (sym->st_shndx == SHN_ABS)
		return ('A');
	if (sym->st_shndx == SHN_COMMON)
		return ('C');
	type = get_section_type(elf, sym->st_shndx);
	sec_name = get_section_name(elf, sym->st_shndx);
	if (type == SHT_PROGBITS)
	{
		if (!ft_strncmp(sec_name, ".text", 5))
			return ('T' + is_local);
		if (!ft_strncmp(sec_name, ".data", 5))
			return ('D' + is_local);
		if (!ft_strncmp(sec_name, ".rodata", 7))
			return ('R' + is_local);
		if (!ft_strncmp(sec_name, ".bss", 4))
			return ('B' + is_local);
		return ('n');
	}
	if (!ft_strncmp(sec_name, ".sdata", 6) || !ft_strncmp(sec_name, ".sbss", 5))
		return ('G' + is_local);
	if (type == SHT_NOBITS)
		return ('B' + is_local);
	if (!ft_strncmp(sec_name, ".debug_", 7))
		return ('N');
	if (type == STT_GNU_IFUNC)
		return ('I' + is_local);
	else
		return '?';
}

Elf64_Sym	*get_symbol_table(t_elf *elf, Elf64_Shdr *shdr)
{

	Elf64_Sym	*symtab;

	symtab = (Elf64_Sym *)seek_elf(elf, shdr->sh_offset, shdr->sh_size);
	if (!symtab)
		return (NULL);
	return (symtab);
}

size_t	load_symbols(t_elf *elf, Elf64_Shdr *shdr, t_symbol *symbols)
{
	Elf64_Sym const	*symtab = get_symbol_table(elf, shdr);
	size_t			i;
	size_t			symcount;
	char			*strtab;
	size_t			strtablen;

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
		symbols[i].entry = &symtab[i];
		symbols[i].value = symtab[i].st_value;
		symbols[i].type = get_symbol_info(elf, &symtab[i]);
		if (symtab[i].st_name < strtablen && strtab[symtab[i].st_name] != '\0')
			symbols[i].name = &strtab[symtab[i].st_name];
		++i;
	}
	return (i);
}

size_t	load_all_symbols(t_elf *elf, t_section *sections, t_symbol *symtab, t_symbol *dynsym)
{
	size_t  loaded;
	size_t  i;

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
