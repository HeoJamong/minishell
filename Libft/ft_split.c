/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:54:09 by jinsecho          #+#    #+#             */
/*   Updated: 2024/08/07 23:41:36 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ptr_free(char **ptr, size_t k)
{
	size_t	i;

	i = 0;
	while (i < k)
	{
		free(ptr[k]);
		k++;
	}
	free(ptr);
}

static char	*ft_strncpy(char *dest, char const *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (dest);
}

static size_t	ptr_count(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	if (c == 0)
		return (0);
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] != c && s[i])
			count++;
		while (s[i] != c && s[i])
			i++;
	}
	return (count);
}

static char	**split_alloc(char **ptr, char const *s, char c, size_t	s_len)
{
	size_t	i;
	size_t	k;
	size_t	len;

	i = 0;
	k = 0;
	while (i < s_len)
	{
		len = 0;
		while (s[i + len] != c && s[i + len])
			len++;
		ptr[k] = (char *)malloc(sizeof(char) * (len + 1));
		if (ptr[k] == NULL)
		{
			ptr_free(ptr, k);
			return (NULL);
		}
		ptr[k] = ft_strncpy(ptr[k], s + i, len);
		i += len;
		while (s[i] == c)
			i++;
		k++;
	}
	ptr[k] = NULL;
	return (ptr);
}

char	**ft_split(char const *s, char c)
{
	char	**ptr;
	size_t	s_len;
	size_t	count;

	if (s == NULL)
		return (NULL);
	count = ptr_count(s, c);
	ptr = (char **)malloc(sizeof(char *) * (count + 1));
	if (ptr == NULL)
		return (NULL);
	while (*s == c && *s != '\0')
		s++;
	s_len = ft_strlen(s);
	ptr = split_alloc(ptr, s, c, s_len);
	if (ptr == NULL)
		return (NULL);
	return (ptr);
}
