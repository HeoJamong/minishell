/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42gyeongsa      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 16:43:31 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/01 20:28:58 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr_arr;
	size_t	i;
	size_t	k;

	if (s == NULL)
		return (NULL);
	k = ft_strlen(s);
	if (k <= start || len == 0)
		return (ft_strdup(""));
	ptr_arr = (char *)malloc(sizeof (char) * len + 1);
	if (ptr_arr == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ptr_arr[i] = s[start + i];
		i++;
	}
	ptr_arr[i] = 0;
	return (ptr_arr);
}

/*
int		main(void)
{
	char *str = ft_substr("123456789", 3, 3);
	printf("%s", str);
}
*/
