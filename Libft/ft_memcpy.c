/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 23:31:53 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/09 19:22:56 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t count)
{
	size_t			i;
	unsigned char	*dest1;

	if (dest == NULL && src == NULL)
		return (NULL);
	i = 0;
	dest1 = (unsigned char *)dest;
	while (i < count)
	{
		dest1[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

/*
char source[ MAX_LEN ] = "This is the source string";
char target[ MAX_LEN ] = "This is the target string";
 
int main(void)
{
	printf( "Before memcpy, target is \"%s\"\n", target );
	ft_memcpy( target, source, sizeof(source));
	printf( "After memcpy, target becomes \"%s\"\n", target );
}
*/
