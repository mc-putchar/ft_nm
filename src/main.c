/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 17:37:36 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/15 13:25:26 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "ft_nm.h"
#include "ft_printf.h"

#define USAGE \
"Usage: %s [option(s)] [file(s)]\n \
 List symbols in [file(s)] (a.out by default).\n \
 Options:\n \
  -a: Display all symbols, including debugger-only symbols.\n \
  -D: Display dynamic symbols instead of normal symbols.\n \
  -g: Display only external symbols.\n \
  -u: Display only undefined symbols.\n \
  -r: Reverse the order of the sort.\n \
  -p: Do not sort the symbols.\n \
  -h: Display help message.\n"

int	main(int ac, char **av)
{
	int		err;
	size_t	done;
	t_input	input;

	input.files = NULL;
	if (parse_input(ac, av, &input))
		return (1);
	if (input.opts & OPT_HELP)
		return (free(input.files), ft_printf(USAGE, av[0]), 0);
	if (!input.file_count && names(DEFAULT_TARGET, input.opts))
		return (1);
	err = 0;
	done = 0;
	if (input.file_count > 1)
		input.opts |= OPT_FILENAME;
	while (done < input.file_count)
	{
		if (names(input.files[done], input.opts))
			++err;
		++done;
	}
	free(input.files);
	return (err);
}
