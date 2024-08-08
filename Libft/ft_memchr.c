/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42gyeongsa      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:23:58 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/01 14:10:50 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *buf, int c, size_t count)
{
	unsigned char	*buf1;
	size_t			i;

	i = 0;
	buf1 = (unsigned char *)buf;
	while (i < count)
	{
		if (buf1[i] == (unsigned char)c)
			return (&buf1[i]);
		i++;
	}
	return (NULL);
}
