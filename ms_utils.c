/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 20:17:30 by jinsecho          #+#    #+#             */
/*   Updated: 2024/10/29 19:14:00 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	line_split_free(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->line_split[i])
		free(cmd->line_split[i++]);
	free(cmd->line_split);
}

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

	tmp = (char *)malloc(sizeof(char) * size + 1);
	if (tmp == NULL)
		exit (EXIT_FAILURE);
	ft_strlcpy(tmp, ptr, ft_strlen(ptr) + 1);
	free(ptr);
	return (tmp);
}

long long	ft_atol(const char *string)
{
	int			minus;
	int			i;
	long long	c;

	minus = 1;
	i = 0;
	c = 0;
	while ((9 <= string[i] && string[i] <= 13) || string[i] == 32)
		i++;
	if (string[i] == '-' || string[i] == '+')
	{
		if (string[i] == '-')
			minus *= -1;
		i++;
	}
	while ('0' <= string[i] && string[i] <= '9')
	{
		c *= 10;
		c = c + (string[i] - '0');
		i++;
	}
	return ((minus * c) % 256);
}
