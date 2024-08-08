/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42gyeongsa      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 12:24:48 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/01 15:05:38 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	void	*ptr_arr;

	ptr_arr = (void *)malloc(num * size);
	if (ptr_arr == NULL)
		return (NULL);
	ft_bzero(ptr_arr, num * size);
	return (ptr_arr);
}
