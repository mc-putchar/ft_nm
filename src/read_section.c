/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_section.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:38:55 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/10 13:41:37 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <stddef.h>
#include "ft_nm.h"

void	*get_section(t_elf *elf, size_t idx, size_t *len)
{
	Elf64_Ehdr const	*ehdr = elf->u_dat.ehdr;
	Elf64_Shdr			*shdr;
	void				*section;
	size_t				offset;

	if (idx >= ehdr->e_shnum)
		return (NULL);
	offset = ehdr->e_shoff + idx * ehdr->e_shentsize;
	shdr = seek_elf(elf, offset, ehdr->e_shentsize);
	if (!shdr)
		return (NULL);
	*len = shdr->sh_size;
	section = seek_elf(elf, shdr->sh_offset, *len);
	if (!section)
		return (NULL);
	return (section);
}
