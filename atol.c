/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atol.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:55:55 by jinsecho          #+#    #+#             */
/*   Updated: 2024/09/14 19:56:25 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long	ft_atol(const char *string)
{
	int			minus;
	int			i;
	long long	c;

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
	return ((minus * c) % 256);
}
