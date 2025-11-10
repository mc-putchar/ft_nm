/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_sheader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:41:22 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/10 13:42:22 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include "ft_nm.h"
#include "ft_printf.h"

static Elf64_Shdr	*get_section_header(t_elf *elf, size_t idx)
{
	Elf64_Ehdr *const	ehdr = elf->u_dat.ehdr;

	return ((Elf64_Shdr *)seek_elf(elf, ehdr->e_shoff + \
			idx * ehdr->e_shentsize, ehdr->e_shentsize));
}

uint32_t	get_section_type(t_elf *elf, size_t idx)
{
	Elf64_Shdr const	*shdr = get_section_header(elf, idx);

	if (!shdr)
		return (0);
	return (shdr->sh_type);
}

uint64_t	get_section_flags(t_elf *elf, size_t idx)
{
	Elf64_Shdr *const	shdr = get_section_header(elf, idx);

	if (!shdr)
		return (0);
	return (shdr->sh_flags);
}

char	*get_section_name(t_elf *elf, size_t idx)
{
	Elf64_Shdr *const	shdr = get_section_header(elf, idx);
	char				*shstrtab;
	size_t				shstrtab_size;

	if (!shdr)
		return (NULL);
	shstrtab_size = 0;
	shstrtab = (char *)get_section(elf, elf->u_dat.ehdr->e_shstrndx, \
				&shstrtab_size);
	if (!shstrtab)
		return (ft_dprintf(STDERR_FILENO, \
		"Error: Failed to read section string table.\n"), NULL);
	return (shstrtab + shdr->sh_name);
}

void	read_section_headers(t_elf *elf, t_section *sections, \
		struct s_symbol_count *symcount)
{
	Elf64_Ehdr const	*ehdr = elf->u_dat.ehdr;
	size_t				i;

	i = 0;
	while (i < ehdr->e_shnum)
	{
		sections[i] = (t_section){0};
		sections[i].shdr = get_section_header(elf, i);
		if (!sections[i].shdr && ++i)
			continue ;
		sections[i].name = get_section_name(elf, i);
		sections[i].size = sections[i].shdr->sh_size;
		sections[i].type = sections[i].shdr->sh_type;
		if (sections[i].type == SHT_SYMTAB)
			symcount->symtab += sections[i].shdr->sh_size / sizeof(Elf64_Sym);
		if (sections[i].type == SHT_DYNSYM)
			symcount->dynsym += sections[i].shdr->sh_size / sizeof(Elf64_Sym);
		sections[i].data = get_section(elf, i, &sections[i].size);
		++i;
	}
}
