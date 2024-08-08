/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42gyeongsa      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:22:24 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/01 14:11:28 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t count)
{
	unsigned char	*dest1;
	size_t			i;

	i = 0;
	dest1 = (unsigned char *)dest;
	if (dest == NULL && src == NULL)
		return (NULL);
	if (dest < src)
	{
		while (i < count)
		{
			dest1[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	else if (dest > src)
	{
		while (0 < count)
		{
			dest1[count - 1] = ((unsigned char *)src)[count - 1];
			count--;
		}
	}
	return (dest);
}
/*
#include <string.h>
#include <stdio.h>
 
#define SIZE    21

char target[SIZE] = "a shiny white sphere";
 
int main( void )
{
	char * p = target + 8;
	char * source = target + 2;
 
	printf( "Before memmove, target is \"%s\"\n", target );
	memmove( p, source, 5 );
  	printf( "After memmove, target becomes \"%s\"\n", target );
}
*/
