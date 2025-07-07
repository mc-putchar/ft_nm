/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 01:55:42 by mcutura           #+#    #+#             */
/*   Updated: 2025/07/07 02:54:57 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include "ft_nm.h"
#include "ft_printf.h"

void	print_raw(void *data, size_t size)
{
	size_t	i;
	char	*byte_data;

	byte_data = (char *)data;
	ft_printf("Raw Data:\n");
	i = 0;
	while (i < size)
	{
		if (!(i & 15) && i != 0)
			ft_printf("\n");
		ft_printf("%02x ", (unsigned char)byte_data[i++]);
	}
	ft_printf("\n\n");
}

char	get_symbol_info(Elf64_Sym *sym)
{
	if (sym->st_shndx == SHN_UNDEF)
		return 'U';
	else if (sym->st_shndx == SHN_ABS)
		return 'A';
	else if (sym->st_shndx == SHN_COMMON)
		return 'C';
	else if (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL)
		return 'T';
	else if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL)
		return 't';
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

void	print_symbols(t_elf *elf, Elf64_Shdr *shdr, char *strtab, \
	size_t strtab_size)
{
	Elf64_Sym	*symtab;
	size_t		i;
	char		sym;
	uint32_t	count;

	symtab = get_symbol_table(elf, shdr);
	if (!symtab)
		return ;
	i = 0;
	count = (uint32_t)shdr->sh_size / sizeof(Elf64_Sym);
	ft_printf("Symbol Table (%u entries):\n", count);
	// print_raw(symtab, count * sizeof(Elf64_Sym));
	i = 0;
	while (i < count)
	{
		sym = get_symbol_info(&symtab[i]);
		if (symtab[i].st_name < strtab_size && strtab[symtab[i].st_name] != '\0')
		{
			if (sym == 'U')
				ft_printf("% 18c %s\n", sym, &strtab[symtab[i].st_name]);
			else 
				ft_printf("%016x %c %s\n", symtab[i].st_value, sym, &strtab[symtab[i].st_name]);
		}
		++i;
	}
}
