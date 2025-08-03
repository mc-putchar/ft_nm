/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 17:37:36 by mcutura           #+#    #+#             */
/*   Updated: 2025/08/03 18:32:51 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include "ft_nm.h"
#include "ft_printf.h"

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
