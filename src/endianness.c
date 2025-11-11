/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endianness.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:02:29 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/11 19:32:47 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

typedef struct s_ec
{
	union
	{
		uint32_t	i;
		char		c[sizeof(uint32_t)];
	}	u_endcheck;
}	t_ec;

int	is_lsb(void)
{
	t_ec	checker;

	checker.u_endcheck.i = 1;
	return (checker.u_endcheck.c[0] == 1);
}

uint16_t	load_uint16(uint16_t val, int swap)
{
	if (!swap)
		return (val);
	return (((val >> 8) & 0xFF) | ((val << 8) & 0xFF00));
}

uint32_t	load_uint32(uint32_t val, int swap)
{
	uint32_t	swapped;

	if (!swap)
		return (val);
	swapped = ((val >> 24) & 0xFF) | ((val >> 8) & 0xFF00);
	swapped |= ((val << 24) & 0xFF000000) | ((val << 8) & 0xFF0000);
	return (swapped);
}

uint64_t	load_uint64(uint64_t val, int swap)
{
	uint64_t	swapped;

	if (!swap)
		return (val);
	swapped = ((val >> 56) & 0xFF) | ((val >> 40) & 0xFF00);
	swapped |= ((val >> 24) & 0xFF0000) | ((val >> 8) & 0xFF000000);
	swapped |= ((val << 8) & 0xFF00000000) | ((val << 24) & 0xFF0000000000);
	swapped |= ((val << 40) & 0xFF000000000000);
	swapped |= ((val << 56) & 0xFF00000000000000);
	return (swapped);
}
