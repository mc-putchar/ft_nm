/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_sheader32.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 15:58:05 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/11 19:09:29 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include "ft_nm.h"
#include "ft_printf.h"

Elf32_Shdr	*get_section_header32(t_elf *elf, size_t idx)
{
	Elf32_Ehdr *const	ehdr = elf->u_dat.ehdr32;
	Elf32_Off const		shoff = load_uint32(ehdr->e_shoff, elf->swap);
	uint16_t const		shentsize = load_uint16(ehdr->e_shentsize, elf->swap);

	return ((Elf32_Shdr *)seek_elf(elf, shoff + idx * shentsize, shentsize));
}

uint32_t	get_section_type32(t_elf *elf, uint32_t idx)
{
	Elf32_Shdr *const	shdr = get_section_header32(elf, idx);
	uint32_t			type;

	if (!shdr)
		return (0);
	type = load_uint32(shdr->sh_type, elf->swap);
	return (type);
}

uint32_t	get_section_flags32(t_elf *elf, uint32_t idx)
{
	Elf32_Shdr *const	shdr = get_section_header32(elf, idx);
	uint32_t			flags;

	if (!shdr)
		return (0);
	flags = load_uint32(shdr->sh_flags, elf->swap);
	return (flags);
}

char	*get_section_name32(t_elf *elf, size_t idx)
{
	Elf32_Shdr *const	shdr = get_section_header32(elf, idx);
	char				*shstrtab;
	uint32_t			shstrtab_size;
	uint16_t			shstrndx;

	if (!shdr)
		return (NULL);
	shstrtab_size = 0;
	shstrndx = load_uint16(elf->u_dat.ehdr32->e_shstrndx, elf->swap);
	shstrtab = (char *)get_section32(elf, shstrndx, &shstrtab_size);
	if (!shstrtab)
		return (ft_dprintf(STDERR_FILENO, \
		"Error: Failed to read section string table.\n"), NULL);
	return (shstrtab + load_uint32(shdr->sh_name, elf->swap));
}

void	read_section_headers32(t_elf *elf, t_section *sections, \
		struct s_symbol_count *symcount)
{
	Elf32_Ehdr *const	ehdr = elf->u_dat.ehdr32;
	uint16_t const		shnum = load_uint16(ehdr->e_shnum, elf->swap);
	uint16_t			i;

	i = 0;
	while (i < shnum)
	{
		sections[i] = (t_section){0};
		sections[i].u_shdr.e32 = get_section_header32(elf, i);
		if (!sections[i].u_shdr.e32 && ++i)
			continue ;
		sections[i].name = get_section_name32(elf, i);
		sections[i].size = load_uint32(sections[i].u_shdr.e32->sh_size, elf->swap);
		sections[i].type = load_uint32(sections[i].u_shdr.e32->sh_type, elf->swap);
		if (sections[i].type == SHT_SYMTAB)
			symcount->symtab += sections[i].size / sizeof(Elf32_Sym);
		if (sections[i].type == SHT_DYNSYM)
			symcount->dynsym += sections[i].size / sizeof(Elf32_Sym);
		sections[i].data = get_section32(elf, i, (uint32_t*)(&sections[i].size));
		++i;
	}
}
