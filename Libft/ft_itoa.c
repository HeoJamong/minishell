/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 21:13:13 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/11 16:43:07 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*minus_sign(char *ptr, int len, long long n)
{
	int		len1;
	int		i;

	len1 = len;
	n *= -1;
	ptr[0] = '-';
	while (len > 0)
	{
		i = n % 10;
		n = n / 10;
		ptr[len] = i + '0';
		len--;
	}
	ptr[len1 + 1] = 0;
	return (ptr);
}

static char	*plus_sign(char *ptr, int len, long long n)
{
	int		len1;
	int		i;

	len1 = len;
	while (len > 0)
	{
		i = n % 10;
		n = n / 10;
		ptr[len - 1] = i + '0';
		len--;
	}
	ptr[len1] = 0;
	return (ptr);
}

static char	*sign(char *ptr, int len, long long n)
{
	if (n < 0)
		ptr = minus_sign(ptr, len, n);
	else if (n > 0)
		ptr = plus_sign(ptr, len, n);
	return (ptr);
}

char	*ft_itoa(int n)
{
	char		*ptr;
	int			len;
	long long	n1;

	len = 1;
	n1 = n;
	if (n1 < 0)
		n1 *= -1;
	while (n1 > 0)
	{
		n1 = n1 / 10;
		len++;
	}
	ptr = (char *)malloc(sizeof(char) * len + 1);
	if (ptr == NULL)
		return (NULL);
	if (n == 0)
	{
		ptr[0] = '0';
		ptr[1] = 0;
		return (ptr);
	}
	ptr = sign(ptr, len - 1, n);
	return (ptr);
}
/*
int		main(void)
{
	char	*str = ft_itoa(1);
	
	printf("%s\n", str);
}
*/
