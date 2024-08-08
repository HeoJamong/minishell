/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42gyeongsa      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:42:36 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/01 14:13:17 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *string, int c)
{
	char	*string1;
	int		len;
	int		len1;

	len = 0;
	string1 = (char *)string;
	while (string1[len])
		len++;
	len1 = len;
	while (0 < len)
	{
		if (string1[len - 1] == c)
			return (&string1[len - 1]);
		len--;
	}
	if (c == 0)
		return (&string1[len1]);
	return (NULL);
}
/*
int main(void)
{
  char buf[SIZE] = "computer program";
  char * ptr;
  int    ch = 'p';

  ptr = strchr( buf, ch );
  printf( "The first occurrence of %c in '%s' is '%s'\n", ch, buf, ptr );

  ptr = strrchr( buf, ch );
  printf( "The last occurrence of %c in '%s' is '%s'\n", ch, buf, ptr );
}
*/
