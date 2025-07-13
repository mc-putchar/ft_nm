/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 17:37:36 by mcutura           #+#    #+#             */
/*   Updated: 2025/07/12 23:25:39 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include "ft_nm.h"

int	main(int ac, char **av)
{
	int			err;
	uint32_t	opts;

	opts = 0;
	if (ac < 2)
		err = names(DEFAULT_TARGET, opts);
	else
		err = names(av[1], opts);
	return (err);
}
