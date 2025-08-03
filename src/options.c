/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 18:32:26 by mcutura           #+#    #+#             */
/*   Updated: 2025/08/03 21:05:00 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bits/pthreadtypes.h>
#include <stddef.h>
#include <unistd.h>
#include "libft_str.h"
#include "ft_printf.h"
#include "ft_nm.h"

static int	parse_arg(char **av, int i, uint32_t *opts)
{
	int	j;

	j = 0;
	while (av[i][++j])
	{
		if (av[i][j] == 'a')
			*opts |= OPT_DBG_SYMS;
		else if (av[i][j] == 'g')
			*opts |= OPT_EXTERNALS;
		else if (av[i][j] == 'u')
			*opts |= OPT_UNDEFINED;
		else if (av[i][j] == 'r')
			*opts |= OPT_REVERSE;
		else if (av[i][j] == 'p')
			*opts |= OPT_NO_SORT;
		else
			return (ft_dprintf(STDERR_FILENO, ERR_UNKOWN_OPT, av[i]), -1);
	}
	return (0);
}

int	parse_options(int ac, char **av, uint32_t *opts, int *flagstop)
{
	int	i;

	i = 0;
	while (++i < ac)
	{
		if (!ft_strncmp(av[i], "--", 3))
		{
			*flagstop = i;
			return (0);
		}
		if (av[i][0] == '-' && !parse_arg(av, i, opts))
			return (-1);
	}
	return (0);
}
