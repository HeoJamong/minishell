/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 00:39:25 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/12 17:11:32 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	check(char str, char *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == str)
			return (0);
		i++;
	}
	return (1);
}

static void	i_check(char *s1, char *set, size_t *f, size_t *len)
{
	while (s1[*f])
	{
		if (check(s1[*f], set) == 1)
			break ;
		(*f)++;
	}
	while (*len > *f)
	{
		if (check(s1[*len], set) == 1)
			break ;
		(*len)--;
	}
}

char	*ft_strtrim(char *s1, char *set)
{
	size_t	f;
	size_t	len;
	char	*s2;

	if (set == NULL || s1 == NULL)
		return (NULL);
	len = ft_strlen(s1) - 1;
	f = 0;
	i_check(s1, set, &f, &len);
	s2 = (char *)malloc(sizeof(char) * (len - f + 2));
	if (s2 == NULL)
		return (NULL);
	ft_strlcpy(s2, &s1[f], len - f + 2);
	return (s2);
}
