/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 09:30:21 by mcutura           #+#    #+#             */
/*   Updated: 2025/07/12 23:37:30 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include "ft_nm.h"
#include "ft_printf.h"

void	print_symbols(t_symbol *symtab, t_symbol *dynsym, struct s_symbol_count *count, uint32_t opts)
{
	size_t	i;

	(void)opts;
	i = 0;
	while (i < count->symtab)
	{
		{
			if (symtab[i].type == 'U' || symtab[i].type == 'W' || symtab[i].type == 'w')
				ft_printf("% 18c %s\n", symtab[i].type, symtab[i].name);
			else
				ft_printf("%016x %c %s\n", symtab[i].value, symtab[i].type, symtab[i].name);
		}
		++i;
	}
	i = 0;
	while (i < count->dynsym)
	{
		{
			if (dynsym[i].type == 'U' || dynsym[i].type == 'W' || dynsym[i].type == 'w')
				ft_printf("% 18c %s\n", dynsym[i].type, dynsym[i].name);
			else
				ft_printf("%016x %c %s\n", dynsym[i].value, dynsym[i].type, dynsym[i].name);
		}
		++i;
	}
}
