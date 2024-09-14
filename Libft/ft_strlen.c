/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:17:11 by jinsecho          #+#    #+#             */
/*   Updated: 2024/09/14 21:51:09 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlen(const char *string)
{
	int	len;

	len = 0;
	if (string == NULL)
		return (0);
	while (*string)
	{
		string++;
		len++;
	}
	return (len);
}
