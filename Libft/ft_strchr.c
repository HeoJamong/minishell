/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42gyeongsa      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 21:22:40 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/01 14:11:56 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *string, int c)
{
	char	*string1;
	int		i;

	i = 0;
	string1 = (char *)string;
	while (string1[i])
	{
		if (string1[i] == c)
			return (&string1[i]);
		i++;
	}
	if (c == 0)
		return (&string1[i]);
	return (NULL);
}
/*
int main(void)
{
	char	buffer1[SIZE] = "computer program";
	char	*ptr;
	int	ch = 'p';

	ptr = ft_strchr( buffer1, ch );
	printf( "The first occurrence of %c in '%s' is '%s'\n",
            ch, buffer1, ptr );
}
*/
