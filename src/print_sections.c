/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_sections.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 01:39:36 by mcutura           #+#    #+#             */
/*   Updated: 2025/07/11 09:49:16 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <stddef.h>
#include "ft_nm.h"
#include "ft_printf.h"

char const	*get_section_type_str(uint32_t type)
{
	static char const	*section_types[] = {
		"NULL", "PROGBITS", "SYMTAB", "STRTAB", "RELA", "HASH", "DYNAMIC", 
		"NOTE", "NOBITS", "REL", "SHLIB", "DYNSYM", "INIT_ARRAY", 
		"FINI_ARRAY", "PREINIT_ARRAY", "GROUP", "SYMTAB_SHNDX", 
		"LOOS", "HIOS", "LOPROC", "HIPROC", "LOUSER", "HIUSER",
	};
	if (type < sizeof(section_types))
		return (section_types[type]);
	return ("UNKNOWN");
}

static void	print_sec_header(Elf64_Shdr *shdr, char *shstrtab, size_t tablen)
{
	if (!shdr)
		return ;
	ft_printf("Section Header:\n");
	if (shstrtab && shdr->sh_name < tablen)
		ft_printf("Section Name: %s\n", &shstrtab[shdr->sh_name]);
	else
		ft_printf("Section Name: [No name]\n");
	ft_printf("  %-34s %x\n", "Name offset:", shdr->sh_name);
	ft_printf("  %-34s %d (%s)\n", "Type:", shdr->sh_type, \
		get_section_type_str(shdr->sh_type));
	ft_printf("  %-34s %#.8x\n", "Flags:", shdr->sh_flags);
	ft_printf("  %-34s %#.8x\n", "Address:", shdr->sh_addr);
	ft_printf("  %-34s %#.8x\n", "Offset:", shdr->sh_offset);
	ft_printf("  %-34s %d (bytes)\n", "Size:", shdr->sh_size);
	ft_printf("  %-34s %d\n", "Link:", shdr->sh_link);
	ft_printf("  %-34s %d\n", "Info:", shdr->sh_info);
	ft_printf("  %-34s %d (bytes)\n", "Address alignment:", shdr->sh_addralign);
	ft_printf("  %-34s %d (bytes)\n", "Entry size:", shdr->sh_entsize);
}

void	print_section_headers(t_elf *elf)
{
	Elf64_Ehdr const	*ehdr = elf->u_dat.ehdr;
	Elf64_Shdr			*shdr;
	char				*shstrtab;
	size_t				shstrtab_size;
	size_t				i;

	ft_printf("Section Headers:\n");
	if (ehdr->e_shnum == 0)
	{
		ft_printf("  No section headers.\n");
		return ;
	}
	shstrtab = get_string_table(elf, ehdr->e_shoff + ehdr->e_shstrndx * ehdr->e_shentsize, ehdr->e_shentsize, &shstrtab_size);
	if (!shstrtab)
		ft_printf("Error: Failed to read section string table.\n");
	i = 0;
	while (i < ehdr->e_shnum)
	{
		shdr = (Elf64_Shdr *)seek_elf(elf, ehdr->e_shoff + i * ehdr->e_shentsize, ehdr->e_shentsize);
		if (!shdr)
			return ;
		if (DEBUG && (shdr->sh_type == SHT_SYMTAB || shdr->sh_type == SHT_STRTAB))
			print_sec_header(shdr, shstrtab, shstrtab_size);
		if (DEBUG && shdr->sh_type == SHT_STRTAB && shdr->sh_size > 0)
			print_strtab(shdr->sh_offset, shdr->sh_size, elf);
		// if (shdr->sh_type == SHT_SYMTAB && shstrtab)
		// 	print_symbols(elf, shdr);
		++i;
	}
}
