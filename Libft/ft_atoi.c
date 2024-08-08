/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42gyeongsa      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:55:55 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/01 14:08:03 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *string)
{
	int	minus;
	int	i;
	int	c;

	minus = 1;
	i = 0;
	c = 0;
	while ((9 <= string[i] && string[i] <= 13) || string[i] == 32)
		i++;
	if (string[i] == '-' || string[i] == '+')
	{
		if (string[i] == '-')
			minus *= -1;
		i++;
	}
	while ('0' <= string[i] && string[i] <= '9')
	{
		c *= 10;
		c = c + (string[i] - '0');
		i++;
	}
	return (minus * c);
}
/*
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char	*s;
	
	s = "	-9885";
	printf("%d\n", atoi(s));
	printf("%d", ft_atoi(s));
	return (0);
}
*/
