/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42gyeongsa      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:55:25 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/01 15:12:16 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *string)
{
	char	*ptr_arr;
	char	*string1;
	int		len;
	int		i;

	i = 0;
	len = 0;
	string1 = (char *)string;
	while (string1[len])
		len++;
	ptr_arr = (char *)malloc(sizeof(char) * (len + 1));
	if (ptr_arr == NULL)
		return (NULL);
	while (i < len)
	{
		ptr_arr[i] = string1[i];
		i++;
	}
	ptr_arr[i] = 0;
	return (ptr_arr);
}
