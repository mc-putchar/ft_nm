/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 19:55:22 by mcutura           #+#    #+#             */
/*   Updated: 2025/08/10 19:55:44 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>
#include <stdlib.h>
#include "ft_nm.h"
#include "ft_printf.h"

static inline int	cleanup(t_elf *elf, t_section *sections, t_symbol *symtab, \
					t_symbol *dynsym)
{
	free(dynsym);
	free(symtab);
	free(sections);
	if (elf)
		munmap(elf->u_dat.addr, elf->size);
	return (0);
}

int	names(char *file, uint32_t opts)
{
	t_elf					elf;
	t_section				*sections;
	t_symbol				*symtab;
	t_symbol				*dynsym;
	struct s_symbol_count	sym_count;

	elf = (t_elf){0};
	sym_count = (struct s_symbol_count){0};
	if (load_file(file, &elf, opts))
		return (-1);
	sections = malloc(elf.u_dat.ehdr->e_shnum * sizeof(t_section));
	if (!sections && !cleanup(&elf, NULL, NULL, NULL))
		return (throw_error(-1, ERR_MALLOC));
	read_section_headers(&elf, sections, &sym_count);
	symtab = malloc(sym_count.symtab * sizeof(t_symbol));
	if (!symtab && !cleanup(&elf, sections, NULL, NULL))
		return (throw_error(-1, ERR_MALLOC));
	dynsym = malloc(sym_count.dynsym * sizeof(t_symbol));
	if (!dynsym && !cleanup(&elf, sections, symtab, NULL))
		return (throw_error(-1, ERR_MALLOC));
	(void)load_all_symbols(&elf, sections, symtab, dynsym);
	print_symbols(symtab, dynsym, &sym_count, opts);
	return (cleanup(&elf, sections, symtab, dynsym));
}
