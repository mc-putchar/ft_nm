/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 09:30:21 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/15 13:21:27 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ft_nm.h"
#include "ft_printf.h"
#include "libft_str.h"

static void	swap_syms(t_symbol *a, t_symbol *b)
{
	t_symbol	tmp;

	tmp.u_entry = a->u_entry;
	tmp.value = a->value;
	tmp.type = a->type;
	tmp.name = a->name;
	tmp.flags = a->flags;
	a->u_entry = b->u_entry;
	a->value = b->value;
	a->type = b->type;
	a->name = b->name;
	a->flags = b->flags;
	b->u_entry = tmp.u_entry;
	b->value = tmp.value;
	b->type = tmp.type;
	b->name = tmp.name;
	b->flags = tmp.flags;
}

static void	sort_symbols(t_symbol *symtab, size_t count, uint32_t opts)
{
	size_t	i;
	size_t	j;
	int		diff;

	if ((opts & OPT_NO_SORT) || count < 2)
		return ;
	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			diff = (symtab[j].name == NULL) - (symtab[i].name == NULL);
			if (symtab[i].name && symtab[j].name)
				diff = ft_strncmp(symtab[i].name, \
				symtab[j].name, ft_strlen(symtab[i].name) + 1);
			if ((diff > 0 && !(opts & OPT_REVERSE)) \
			|| (diff < 0 && opts & OPT_REVERSE) || (!diff && \
			((symtab[i].value > symtab[j].value && !(opts & OPT_REVERSE)) \
			|| (symtab[i].value < symtab[j].value && (opts & OPT_REVERSE)))))
				swap_syms(&symtab[i], &symtab[j]);
			++j;
		}
		++i;
	}
}

static void	print_symbol(t_symbol *symbol, uint32_t opts)
{
	if (symbol->flags & SYM_IS_UNDEF)
	{
		if (opts & OPT_32BIT)
			ft_printf("%10c", symbol->type);
		else
			ft_printf("%18c", symbol->type);
	}
	else if (opts & OPT_UNDEFINED)
		return ;
	else
	{
		if (opts & OPT_32BIT)
			ft_printf("%08x %c", symbol->value, symbol->type);
		else
			ft_printf("%016x %c", symbol->value, symbol->type);
	}
	if (symbol->name)
		ft_printf(" %s\n", symbol->name);
	else
		ft_printf(" \n");
}

void	print_symbols(char const *file, t_symbol *syms, size_t count, \
	uint32_t opts)
{
	size_t	i;

	if (opts & OPT_FILENAME)
		ft_printf("\n%s:\n", file);
	if (!count)
	{
		ft_dprintf(STDERR_FILENO, ERR_NO_SYM, file);
		return ;
	}
	sort_symbols(syms, count, opts);
	i = 0;
	while (i < count)
	{
		if ((!(syms[i].flags & SYM_IS_DBG) || (opts & OPT_DBG_SYMS)) \
		&& (!(opts & OPT_EXTERNALS) || syms[i].flags & SYM_IS_EXT))
		{
			if (syms[i].name || syms[i].value \
			|| syms[i].type != 'U')
				print_symbol(&syms[i], opts);
		}
		++i;
	}
}
