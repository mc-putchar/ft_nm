/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_sheader32.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 15:58:05 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/14 23:07:47 by mcutura          ###   ########.fr       */
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

int	read_section_headers32(t_elf *elf, t_section *sec, \
		struct s_symbol_count *symcount)
{
	Elf32_Ehdr *const	ehdr = elf->u_dat.ehdr32;
	uint16_t const		shnum = load_uint16(ehdr->e_shnum, elf->swap);
	uint16_t			i;

	i = 0;
	while (i < shnum)
	{
		sec[i] = (t_section){0};
		sec[i].u_shdr.e32 = get_section_header32(elf, i);
		if (!sec[i].u_shdr.e32)
			return (-1);
		sec[i].name = get_section_name32(elf, i);
		sec[i].size = load_uint32(sec[i].u_shdr.e32->sh_size, elf->swap);
		sec[i].type = load_uint32(sec[i].u_shdr.e32->sh_type, elf->swap);
		if (sec[i].type == SHT_SYMTAB)
			symcount->symtab += sec[i].size / sizeof(Elf32_Sym);
		if (sec[i].type == SHT_DYNSYM)
			symcount->dynsym += sec[i].size / sizeof(Elf32_Sym);
		sec[i].data = get_section32(elf, i, (uint32_t *)(&sec[i].size));
		++i;
	}
	return (0);
}
