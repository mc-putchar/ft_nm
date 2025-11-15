/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 19:55:22 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/15 14:09:56 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
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

int	prep_sections(t_elf *elf, t_section **sections, \
	struct s_symbol_count *sym_count)
{
	uint16_t	shnum;
	int			result;

	if (elf->is64)
		shnum = load_uint16(elf->u_dat.ehdr->e_shnum * sizeof(t_section), \
					elf->swap);
	else
		shnum = load_uint16(elf->u_dat.ehdr32->e_shnum * sizeof(t_section), \
					elf->swap);
	*sections = malloc(shnum * sizeof(t_section));
	if (!*sections && !cleanup(elf, NULL, NULL, NULL))
		return (throw_error(-1, ERR_MALLOC));
	if (elf->is64)
		result = read_section_headers(elf, *sections, sym_count);
	else
		result = read_section_headers32(elf, *sections, sym_count);
	if (result)
	{
		ft_dprintf(STDERR_FILENO, ERR_BAD_ELF, elf->filename);
		cleanup(elf, *sections, NULL, NULL);
		return (-1);
	}
	return (0);
}

int	names(char const *file, uint32_t opts)
{
	t_section	*sections;
	t_elf		elf;
	t_symbols	symbols;

	elf = (t_elf){0};
	symbols = (t_symbols){0};
	if (load_file(file, &elf) || prep_sections(&elf, &sections, &symbols.count))
		return (-1);
	symbols.symtab = malloc(symbols.count.symtab * sizeof(t_symbol));
	if (!symbols.symtab && !cleanup(&elf, sections, NULL, NULL))
		return (throw_error(-1, ERR_MALLOC));
	symbols.dynsym = malloc(symbols.count.dynsym * sizeof(t_symbol));
	if (!symbols.dynsym && !cleanup(&elf, sections, symbols.symtab, NULL))
		return (throw_error(-1, ERR_MALLOC));
	if (load_all_symbols(&elf, sections, symbols.symtab, symbols.dynsym) < 0)
	{
		ft_dprintf(STDERR_FILENO, ERR_BAD_ELF, file);
		return (cleanup(&elf, sections, symbols.symtab, symbols.dynsym), -1);
	}
	opts |= (OPT_32BIT * (!elf.is64));
	if (opts & OPT_DYNAMIC)
		print_symbols(file, symbols.dynsym, symbols.count.dynsym, opts);
	else
		print_symbols(file, symbols.symtab, symbols.count.symtab, opts);
	return (cleanup(&elf, sections, symbols.symtab, symbols.dynsym));
}
