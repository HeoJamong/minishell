/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:17:11 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/17 20:36:07 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlen(const char *string)
{
	int	len;

	len = 0;
	if (string == NULL)
		return (len);
	while (*string)
	{
		string++;
		len++;
	}
	return (len);
}

int	sl(const char *string)
{
	int	len;

	len = 0;
	if (string == NULL)
		return (len);
	while (*string)
	{
		string++;
		len++;
	}
	return (len);
}
