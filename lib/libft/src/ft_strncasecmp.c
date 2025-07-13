/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncasecmp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 21:39:24 by mcutura           #+#    #+#             */
/*   Updated: 2025/07/13 16:32:53 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

int	ft_strncasecmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	int		c1;
	int		c2;

	i = 0;
	while (i < n)
	{
		c1 = ft_tolower((unsigned char)s1[i]);
		c2 = ft_tolower((unsigned char)s2[i]);
		if (c1 != c2)
			return (c1 - c2);
		++i;
	}
	return (0);
}
