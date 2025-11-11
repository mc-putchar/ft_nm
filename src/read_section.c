/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_section.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:38:55 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/11 16:59:39 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <stddef.h>
#include "ft_nm.h"

void	*get_section(t_elf *elf, size_t idx, size_t *len)
{
	Elf64_Ehdr const	*ehdr = elf->u_dat.ehdr;
	uint16_t const		shnum = load_uint16(ehdr->e_shnum, elf->swap);
	Elf64_Shdr			*shdr;
	Elf64_Off			offset;

	if (idx >= shnum)
		return (NULL);
	shdr = get_section_header(elf, idx);
	if (!shdr)
		return (NULL);
	*len = load_uint64(shdr->sh_size, elf->swap);
	offset = load_uint64(shdr->sh_offset, elf->swap);
	return (seek_elf(elf, offset, *len));
}

void	*get_section32(t_elf *elf, uint32_t idx, uint32_t *len)
{
	Elf32_Ehdr const	*ehdr = elf->u_dat.ehdr32;
	uint16_t const		shnum = load_uint16(ehdr->e_shnum, elf->swap);
	Elf32_Shdr			*shdr;
	Elf32_Off			offset;

	if (idx >= shnum)
		return (NULL);
	shdr = get_section_header32(elf, idx);
	if (!shdr)
		return (NULL);
	*len = load_uint32(shdr->sh_size, elf->swap);
	offset = load_uint32(shdr->sh_offset, elf->swap);
	return (seek_elf(elf, offset, *len));
}
