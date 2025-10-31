/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 09:30:21 by mcutura           #+#    #+#             */
/*   Updated: 2025/08/10 17:16:43 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include <sys/stat.h>
#include "ft_nm.h"
#include "ft_printf.h"
#include "libft.h"
#include "libft_str.h"

static void	swap_syms(t_symbol *a, t_symbol *b)
{
	t_symbol	tmp;

	tmp.entry = a->entry;
	tmp.value = a->value;
	tmp.type = a->type;
	tmp.name = a->name;
	tmp.flags = a->flags;
	a->entry = b->entry;
	a->value = b->value;
	a->type = b->type;
	a->name = b->name;
	a->flags = b->flags;
	b->entry = tmp.entry;
	b->value = tmp.value;
	b->type = tmp.type;
	b->name = tmp.name;
	b->flags = tmp.flags;
}

static int	alphacmp(char const *sym1, char const *sym2)
{
	int		c1;
	int		c2;

	while (*sym1 && *sym2)
	{
		while (!ft_isalnum(*sym1))
			++sym1;
		while (!ft_isalnum(*sym2))
			++sym2;
		if (!*sym1 || !*sym2)
			return (*sym1 - *sym2);
		c1 = ft_tolower((unsigned char)*sym1);
		c2 = ft_tolower((unsigned char)*sym2);
		if (c1 != c2)
			return (c1 - c2);
		++sym1;
		++sym2;
	}
	if (!*sym1)
		return (-1);
	if (!*sym2)
		return (1);
	return (0);
}

// TODO: fix reverse sorting bugs
static void	sort_symbols(t_symbol *symtab, \
	struct s_symbol_count *count, uint32_t opts)
{
	size_t	i;
	size_t	j;
	int		diff;

	if ((opts & OPT_NO_SORT) || count->symtab < 2)
		return ;
	i = 0;
	while (i < count->symtab - 1)
	{
		j = i + 1;
		while (j < count->symtab)
		{
			diff = (symtab[j].name == NULL) - (symtab[i].name == NULL);
			if (symtab[i].name && symtab[j].name)
			{
			    // TODO: Sorting depends on locale settings,
				// test and figure out which sorting strategy to use
				diff = alphacmp(symtab[i].name, symtab[j].name);
				diff = ft_strncmp(symtab[i].name, symtab[j].name, ft_strlen(symtab[i].name));
				// if (!diff)
				/* diff = ft_strncasecmp(symtab[i].name, symtab[j].name, \
						ft_strlen(symtab[i].name));
						*/
			}
			if ((diff > 0 && !(opts & OPT_REVERSE)) \
			|| (diff < 0 && opts & OPT_REVERSE))
				swap_syms(&symtab[i], &symtab[j]);
			++j;
		}
		++i;
	}
}

static void print_symbol(t_symbol *symbol, uint32_t opts)
{
	if (symbol->flags & SYM_IS_UNDEF)
	    ft_printf("% 18c", symbol->type);
	else if (opts & OPT_UNDEFINED)
		return ;
	else
		ft_printf("%016x %c", symbol->value, symbol->type);
	if (symbol->name)
    	ft_printf(" %s\n", symbol->name);
	else
		ft_printf(" \n");
}

void	print_symbols(t_symbol *symtab, t_symbol *dynsym, \
	struct s_symbol_count *count, uint32_t opts)
{
	size_t	i;

	if (!count->symtab)
	{
		print_error(ERR_NO_SYM);
		return ;
	}
	sort_symbols(symtab, count, opts);
	i = 0;
	while (i < count->symtab)
	{
		if ((!(symtab[i].flags & SYM_IS_DBG) || (opts & OPT_DBG_SYMS)) \
		&& (!(opts & OPT_EXTERNALS) || symtab[i].flags & SYM_IS_EXT))
		{
			if (symtab[i].name || symtab[i].value || symtab[i].type != 'U')
                print_symbol(&symtab[i], opts);
		}
		++i;
	}
	(void)dynsym;
}
