/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:29:30 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/12 00:37:58 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*s1;
	unsigned int	i;

	if (s == NULL || f == NULL)
		return (NULL);
	i = 0;
	s1 = ft_strdup(s);
	if (s1 == NULL)
		return (NULL);
	while (s1[i])
	{
		s1[i] = f(i, s1[i]);
		i++;
	}
	return (s1);
}
