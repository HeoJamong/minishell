/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 18:36:33 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/09 19:43:30 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	k;
	char	*big1;
	char	*little1;

	if (big == NULL && len == 0)
		return (NULL);
	big1 = (char *)big;
	little1 = (char *)little;
	if (*little == 0)
		return (big1);
	i = 0;
	while (big1[i] && i < len)
	{
		k = 0;
		while (big1[i + k] == little1[k] && i + k < len)
		{
			k++;
			if (little1[k] == 0)
				return (&big1[i]);
		}
		i++;
	}
	return (NULL);
}
