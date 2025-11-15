/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_symbol_type32.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:09:21 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/15 00:01:00 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <sys/types.h>
#include "ft_nm.h"
#include "libft_str.h"

static int	get_well_known(Elf32_Sym const *sym, int is_local, int bind, \
	int swap)
{
	Elf32_Addr const	value = load_uint32(sym->st_value, swap);
	uint16_t const		shndx = load_uint16(sym->st_shndx, swap);
	int const			info = ELF32_ST_TYPE(sym->st_info);

	if (info == STT_GNU_IFUNC)
		return ('i');
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

static int	get_symbol_info(t_elf *elf, Elf32_Sym const *sym, int is_local)
{
	uint16_t const	shndx = load_uint16(sym->st_shndx, elf->swap);
	uint32_t const	flags = get_section_flags32(elf, shndx);
	uint32_t const	type = get_section_type32(elf, shndx);
	char *const		sec_name = get_section_name32(elf, shndx);

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
	return (try_guess_type_from_name(sec_name, is_local));
}

int	get_symbol_type32(t_elf *elf, Elf32_Sym const *sym)
{
	int const	bind = ELF32_ST_BIND(sym->st_info);
	int const	is_local = (ELF32_ST_BIND(sym->st_info) == STB_LOCAL) * 32;
	int			type;

	type = get_well_known(sym, is_local, bind, elf->swap);
	if (type)
		return (type);
	return (get_symbol_info(elf, sym, is_local));
}
