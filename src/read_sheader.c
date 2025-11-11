/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_sheader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:41:22 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/11 19:36:26 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include "ft_nm.h"
#include "ft_printf.h"

Elf64_Shdr	*get_section_header(t_elf *elf, size_t idx)
{
	Elf64_Ehdr *const	ehdr = elf->u_dat.ehdr;
	Elf64_Off const		shoff = load_uint64(ehdr->e_shoff, elf->swap);
	uint16_t const		shentsize = load_uint16(ehdr->e_shentsize, elf->swap);

	return ((Elf64_Shdr *)seek_elf(elf, shoff + idx * shentsize, shentsize));
}

uint32_t	get_section_type(t_elf *elf, size_t idx)
{
	Elf64_Shdr *const	shdr = get_section_header(elf, idx);
	uint32_t			type;

	if (!shdr)
		return (0);
	type = load_uint32(shdr->sh_type, elf->swap);
	return (type);
}

uint64_t	get_section_flags(t_elf *elf, size_t idx)
{
	Elf64_Shdr *const	shdr = get_section_header(elf, idx);
	uint64_t			flags;

	if (!shdr)
		return (0);
	flags = load_uint64(shdr->sh_flags, elf->swap);
	return (flags);
}

char	*get_section_name(t_elf *elf, size_t idx)
{
	Elf64_Shdr *const	shdr = get_section_header(elf, idx);
	char				*shstrtab;
	size_t				shstrtab_size;
	uint16_t			shstrndx;

	if (!shdr)
		return (NULL);
	shstrtab_size = 0;
	shstrndx = load_uint16(elf->u_dat.ehdr->e_shstrndx, elf->swap);
	shstrtab = (char *)get_section(elf, shstrndx, &shstrtab_size);
	if (!shstrtab)
		return (ft_dprintf(STDERR_FILENO, \
		"Error: Failed to read section string table.\n"), NULL);
	return (shstrtab + load_uint32(shdr->sh_name, elf->swap));
}

void	read_section_headers(t_elf *elf, t_section *sections, \
		struct s_symbol_count *symcount)
{
	Elf64_Ehdr *const	ehdr = elf->u_dat.ehdr;
	uint16_t const		shnum = load_uint16(ehdr->e_shnum, elf->swap);
	uint16_t			i;

	i = 0;
	while (i < shnum)
	{
		sections[i] = (t_section){0};
		sections[i].u_shdr.e64 = get_section_header(elf, i);
		if (!sections[i].u_shdr.e64 && ++i)
			continue ;
		sections[i].name = get_section_name(elf, i);
		sections[i].size = load_uint64(sections[i].u_shdr.e64->sh_size, \
							elf->swap);
		sections[i].type = load_uint32(sections[i].u_shdr.e64->sh_type, \
							elf->swap);
		if (sections[i].type == SHT_SYMTAB)
			symcount->symtab += sections[i].size / sizeof(Elf64_Sym);
		if (sections[i].type == SHT_DYNSYM)
			symcount->dynsym += sections[i].size / sizeof(Elf64_Sym);
		sections[i].data = get_section(elf, i, &sections[i].size);
		++i;
	}
}
