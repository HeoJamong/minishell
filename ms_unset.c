/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:29:07 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/23 21:42:40 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_envplen(char	**envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

int	ft_contains(char *str, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], str, ft_strlen(str)) && \
		envp[i][strlen(str)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void	play_unset(char ***new, int len, int i, t_cmd *cmd)
{
	int	index;

	index = 0;
	while (index < i)
	{
		(*new)[index] = ft_strdup(cmd->envp[index]);
		free(cmd->envp[index]);
		index++;
	}
	free(cmd->envp[i]);
	while (i + 1 < len)
	{
		(*new)[index] = ft_strdup(cmd->envp[i + 1]);
		free(cmd->envp[i + 1]);
		index++;
		i++;
	}
	(*new)[index] = NULL;
	free(cmd->envp);
	cmd->envp = (*new);
}

int	ft_unset(char *str, t_cmd *cmd)
{
	char	**new;
	int		len;
	int		i;

	len = ft_envplen(cmd->envp);
	i = ft_contains(str, cmd->envp);
	if (i == -1)
		return (0);
	else
	{
		new = malloc(sizeof(char *) * (len));
		play_unset(&new, len, i, cmd);
	}
	return (1);
}
