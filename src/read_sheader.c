/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_sheader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:41:22 by mcutura           #+#    #+#             */
/*   Updated: 2025/07/08 22:50:36 by mcutura          ###   ########.fr       */
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
	Elf64_Ehdr const	*ehdr = elf->u_dat.ehdr;

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

char	*get_section_name(t_elf *elf, size_t idx)
{
	Elf64_Shdr const	*shdr = get_section_header(elf, idx);
	char				*shstrtab;
	size_t				shstrtab_size;

	if (!shdr)
		return (NULL);
	shstrtab_size = 0;
	shstrtab = (char *)get_section(elf, elf->u_dat.ehdr->e_shstrndx, &shstrtab_size);
	if (!shstrtab)
		return (ft_dprintf(STDERR_FILENO, "Error: Failed to read section string table.\n"), NULL);
	return (shstrtab + shdr->sh_name);
}

int	read_section_headers(t_elf *elf)
{
	Elf64_Ehdr const	*ehdr = elf->u_dat.ehdr;
	Elf64_Shdr			*shdr;
	char				*shstrtab;
	size_t				shstrtab_size;
	size_t				i;

	if (ehdr->e_shnum == 0)
	{
		ft_printf("  No section headers found.\n");
		return (-1);
	}
	shstrtab_size = 0;
	shstrtab = (char *)get_section(elf, ehdr->e_shstrndx, &shstrtab_size);
	if (!shstrtab)
		ft_printf("Error: Failed to read section string table.\n");
	i = 0;
	while (i < ehdr->e_shnum)
	{
		shdr = get_section_header(elf, i);
		if (!shdr)
			return (-1);
		if (shdr->sh_type == SHT_SYMTAB && shstrtab)
			print_symbols(elf, shdr);
		++i;
	}
	return (0);
}
