/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:43:36 by jheo              #+#    #+#             */
/*   Updated: 2024/09/14 17:36:00 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

char	**set_env(char **envp)
{
	char	**new;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	new = malloc(sizeof(char *) * (i + 1));
	if (new == NULL)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new[i] = ft_strdup(envp[i]);
		i++;
	}
	new[i] = NULL;
	return (new);
}

int	check_env(char *str, char *env)
{
	int	i;

	i = 0;
	while (str[i] && env[i] && (str[i] == env[i]) && \
	(str[i] != '=') && (env[i] != '='))
		i++;
	if (i == 0)
		return (0);
	if (((str[i] == '=') && (env[i] == '=')))
		return (1);
	return (0);
}

void	new_export(char *str, char **new, int index)
{
	new[index] = ft_strdup(str);
	new[index + 1] = NULL;
}

int	ft_export(char *str, t_cmd *cmd)
{
	int		i;
	char	**new;

	if (str[0] == '=' || str[0] == '\0' || ft_isdigit(str[0]))
		return (0);
	i = 0;
	while ((cmd->envp)[i])
	{
		if (check_env(str, (cmd->envp)[i]))
		{
			free((cmd->envp)[i]);
			(cmd->envp)[i] = ft_strdup(str);
			return (1);
		}
		i++;
	}
	new = malloc(sizeof(char *) * (i + 2));
	if (new == NULL)
		exit(1);
	i = 0;
	while (cmd->envp[i])
	{
		new[i] = ft_strdup((cmd->envp)[i]);
		free((cmd->envp)[i]);
		i++;
	}
	new_export(str, new, i);
	free(cmd->envp);
	cmd->envp = new;
	return (1);
}
