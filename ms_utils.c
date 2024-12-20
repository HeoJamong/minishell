/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 20:17:30 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 15:20:23 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_line_split_free(char **tmp)
{
	int	i;

	i = 0;
	while (tmp[i])
		free(tmp[i++]);
	free(tmp);
}

char	*ft_envchr(char *env, char *str)
{
	int	i;

	i = 0;
	while (env[i] != '=' && env[i])
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

long long	ft_atoll(const char *str)
{
	int			minus;
	int			i;
	long long	c;

	minus = 1;
	i = 0;
	c = 0;
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			minus *= -1;
		i++;
	}
	while ('0' <= str[i] && str[i] <= '9')
	{
		c *= 10;
		c = c + (str[i] - '0');
		i++;
	}
	return ((minus * c) % 256);
}
void	count_ca_cnt(t_plst *l)
{
	while (l->pipe_split[l->ca_cnt])
		l->ca_cnt++;
}
