/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_symbol_type.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 21:31:09 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/11 19:39:44 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <sys/types.h>
#include "ft_nm.h"
#include "libft_str.h"

static int	get_well_known(Elf64_Sym const *sym, int is_local, int bind, \
	int swap)
{
	Elf64_Addr const	value = load_uint64(sym->st_value, swap);
	uint16_t const		shndx = load_uint16(sym->st_shndx, swap);
	int const			info = ELF64_ST_TYPE(sym->st_info);

	if (bind == STB_WEAK)
	{
		if (info == STT_OBJECT)
			return ('V' + 32 * (value == 0));
		return ('W' + 32 * (value == 0));
	}
	if (bind == STB_GNU_UNIQUE)
		return ('u');
	if (shndx == SHN_ABS)
		return ('A' + is_local);
	if (shndx == SHN_COMMON)
		return ('C');
	if (shndx == SHN_UNDEF)
		return ('U');
	return (0);
}

static int	try_guess_type_from_name(char *const sec_name, int is_local)
{
	if (sec_name)
	{
		if (!ft_strncmp(sec_name, ".text", 5))
			return ('T' + is_local);
		if (!ft_strncmp(sec_name, ".data", 5))
			return ('D' + is_local);
		if (!ft_strncmp(sec_name, ".rodata", 7))
			return ('R' + is_local);
		if (!ft_strncmp(sec_name, ".bss", 4))
			return ('B' + is_local);
		if (!ft_strncmp(sec_name, ".sdata", 6) \
		|| !ft_strncmp(sec_name, ".sbss", 5))
			return ('G' + is_local);
		if (!ft_strncmp(sec_name, ".debug_", 7))
			return ('N');
	}
	return ('?');
}

static int	get_symbol_info(t_elf *elf, Elf64_Sym const *sym, int is_local)
{
	uint16_t const	shndx = load_uint16(sym->st_shndx, elf->swap);
	uint64_t const	flags = get_section_flags(elf, shndx);
	uint32_t const	type = get_section_type(elf, shndx);
	char *const		sec_name = get_section_name(elf, shndx);

	if (!(flags & SHF_ALLOC))
		return ('N' + is_local);
	if (type == SHT_NOBITS && (flags & SHF_WRITE))
		return ('B' + is_local);
	else if (flags & SHF_EXECINSTR)
		return ('T' + is_local);
	else if (!(flags & SHF_WRITE))
		return ('R' + is_local);
	else if ((flags & SHF_WRITE) && !(flags & SHF_EXECINSTR))
		return ('D' + is_local);
	else if (type == STT_GNU_IFUNC)
		return ('I' + is_local);
	return (try_guess_type_from_name(sec_name, is_local));
}

int	get_symbol_type(t_elf *elf, Elf64_Sym const *sym)
{
	int const	bind = ELF64_ST_BIND(sym->st_info);
	int const	is_local = (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) * 32;
	int			type;

	type = get_well_known(sym, is_local, bind, elf->swap);
	if (type)
		return (type);
	return (get_symbol_info(elf, sym, is_local));
}
