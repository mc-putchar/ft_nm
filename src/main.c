/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 17:37:36 by mcutura           #+#    #+#             */
/*   Updated: 2025/07/13 17:12:05 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include "libft_str.h"
#include "ft_nm.h"
#include "ft_printf.h"

int	parse_options(int ac, char **av, uint32_t *opts, int *flagstop)
{
	int	i;
	int	j;

	i = 0;
	while (++i < ac)
	{
		if (!ft_strncmp(av[i], "--", 3))
		{
			*flagstop = i;
			return (0);
		}
		j = 0;
		while (av[i][0] == '-' && av[i][++j])
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
	}
	return (0);
}

int	main(int ac, char **av)
{
	int			err;
	int			i;
	int			proc;
	uint32_t	opts;
	int			flagstop;

	opts = 0;
	flagstop = ac;
	if (parse_options(ac, av, &opts, &flagstop))
		return (1);
	err = 0;
	proc = 0;
	i = 0;
	while (++i < ac)
	{
		if (av[i][0] == '-' && i <= flagstop)
			continue ;
		if (DEBUG)
			ft_printf("Processing file: %s\n", av[i]);
		if (++proc && names(av[i], opts))
			err = 1;
	}
	if (!proc && names(DEFAULT_TARGET, opts))
		return (1);
	return (err);
}
