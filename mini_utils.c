/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 20:17:30 by jinsecho          #+#    #+#             */
/*   Updated: 2024/09/03 22:49:48 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char	*ft_envchr(char *env, char *str)
{
	int	i;

	i = 0;
	while (env[i] != '=')
	{
		if (env[i] != str[i])
			return (NULL);
		i++;
	}
	if (env[i] == '=' && str[i] == 0)
		return (env);
	else
		return (NULL);
}

char	*ft_realloc(char *ptr, int size)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = (char *)malloc(sizeof(char) * size + 1);
	if (tmp == NULL)
		exit (EXIT_FAILURE);
	ft_strlcpy(tmp, ptr, ft_strlen(ptr) + 1);
	free(ptr);
	return (tmp);
}