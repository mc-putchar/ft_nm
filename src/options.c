/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 18:32:26 by mcutura           #+#    #+#             */
/*   Updated: 2025/08/10 19:41:25 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft_str.h"
#include "ft_printf.h"
#include "ft_nm.h"

// Possible options:
// -a: Display debugger-only symbols.
// -g: Display only external symbols.
// -u: Display only undefined symbols.
// -r: Reverse the sense of the sort.
// -p: Do not sort the symbols.
// -h: Display help message.
static int	parse_arg(char **av, uint32_t *opts, int i)
{
	static char const *const	options = "agurph";
	char						*opt;
	int							j;

	j = 0;
	while (av[i][++j])
	{
		opt = ft_strchr(options, av[i][j]);
		if (!opt)
			return (ft_dprintf(STDERR_FILENO, ERR_UNKOWN_OPT, av[i][j]), -1);
		*opts |= (1 << (opt - options));
	}
	return (0);
}

static int	parse_options(int ac, char **av, uint32_t *opts, int *flagstop)
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
		if (av[i][0] == '-' && parse_arg(av, opts, i))
			return (-1);
	}
	return (0);
}

static int	parse_files(int ac, char **av, t_input *input)
{
	int		i;
	size_t	j;

	i = 0;
	while (++i < ac)
		if (av[i][0] != '-' || i > input->flagstop)
			++input->file_count;
	if (input->file_count)
	{
		input->files = malloc(sizeof(char *) * input->file_count);
		if (!input->files)
			return (throw_error(-1, ERR_MALLOC));
		i = 0;
		j = 0;
		while (++i < ac && j < input->file_count)
			if (av[i][0] != '-' || i > input->flagstop)
				input->files[j++] = av[i];
	}
	return (0);
}

int	parse_input(int ac, char **av, t_input *input)
{
	input->flagstop = ac;
	input->opts = 0;
	input->file_count = 0;
	input->files = NULL;
	if (parse_options(ac, av, &input->opts, &input->flagstop) \
	|| parse_files(ac, av, input))
		return (-1);
	return (0);
}
