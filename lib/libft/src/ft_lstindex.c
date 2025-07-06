/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstindex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:46:38 by mcutura           #+#    #+#             */
/*   Updated: 2025/07/06 22:49:05 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft_lst.h"

t_list	*ft_lstindex(t_list *lst, size_t index)
{
	size_t	i;

	i = 0;
	if (!index)
		return (lst);
	while (lst && ++i < index)
		lst = lst->next;
	if (i == index)
		return (lst);
	return (NULL);
}
