/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 17:20:45 by jinsecho          #+#    #+#             */
/*   Updated: 2024/05/11 16:53:37 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	join_input(char *ptr, char const *s1, char const *s2)
{
	size_t	i;
	size_t	s1_len;
	size_t	s2_len;

	i = 0;
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	while (i < s1_len)
	{
		ptr[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < s2_len)
	{
		ptr[s1_len + i] = s2[i];
		i++;
	}
	ptr[s1_len + i] = 0;
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*ptr_arr;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	ptr_arr = (char *)malloc(sizeof(char) * (s1_len + s2_len) + 1);
	if (ptr_arr == NULL)
		return (NULL);
	join_input(ptr_arr, s1, s2);
	return (ptr_arr);
}
