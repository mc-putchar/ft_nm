/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_strings.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 01:40:48 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/11 18:05:36 by mcutura          ###   ########.fr       */
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

// void	print_strtab(size_t offset, size_t size, t_elf *elf)
// {
// 	char	*strtab;
// 	size_t	start;
// 	size_t	end;
// 	int		i;
// 
// 	strtab = (char *)seek_elf(elf, offset, size);
// 	if (!strtab)
// 	{
// 		ft_printf("Error: Failed to read string table at offset %#x\n", offset);
// 		return ;
// 	}
// 	ft_printf("String Table:\n");
// 	i = 0;
// 	start = 0;
// 	end = start;
// 	while (end < size)
// 	{
// 		if (strtab[end++] == '\0')
// 		{
// 			ft_printf("  [% 3d] %s\n", i++, &strtab[start]);
// 			start = end;
// 		}
// 	}
// 	ft_printf("\n");
// }

char	*get_string_table(t_elf *elf, size_t offset, size_t size, size_t *len)
{
	Elf64_Shdr	*shdr;
	char		*strtab;

	shdr = seek_elf(elf, offset, size);
	if (!shdr)
	{
		ft_printf("Error: Failed to read string table at offset %#x\n", offset);
		return (NULL);
	}
	else
	{
		*len = shdr->sh_size;
		strtab = (char *)seek_elf(elf, shdr->sh_offset, *len);
		if (!strtab)
		{
			ft_printf("Error: Failed to read string table at offset %#x\n", \
				shdr->sh_offset);
			return (NULL);
		}
		if (shdr->sh_type != SHT_STRTAB)
			ft_printf("Warning: Section type is not SHT_STRTAB (%s)\n", \
				get_section_type_str(shdr->sh_type));
	}
	return (strtab);
}
