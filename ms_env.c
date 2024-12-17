/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:43:36 by jheo              #+#    #+#             */
/*   Updated: 2024/12/17 11:46:09 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	new_export(char *str, char **new, int index)
{
	new[index] = ft_strdup(str);
	new[index + 1] = NULL;
}


int	e_pointer_export_check(int e_pointer, char *str)
{
	int	i;

	i = 0;
	if (e_pointer)
	{
		while(i < e_pointer)
		{
			if (check_ispossible_export(str[i]) == 0)
				return(0);
			i++;
		}
	}
	else
	{
		while (str[i])
		{
			if (check_ispossible_export(str[i]) == 0)
				return(0);
			i++;
		}
	}
	return (1);
}

int	change_env(t_cmd *cmd, char *str, int *i)
{
	while ((cmd->envp)[*i])
	{
		if (check_env(str, (cmd->envp)[*i]))
		{
			free((cmd->envp)[*i]);
			(cmd->envp)[*i] = ft_strdup(str);
			return (1);
		}
		(*i)++;
	}
	return (0);
}

int	ft_export(char *str, t_cmd *cmd)
{
	int		i;
	int		e_pointer;
	char	**new;

	i = 0;
	if (!(str[0] == '_' || ft_isalpha(str[0])))
		return (0);
	e_pointer = find_char_index(str, '=');
	if (e_pointer_export_check(e_pointer, str) == 0)
		return (0);
	if (change_env(cmd, str, &i) == 1)
		return (1);
	new = malloc(sizeof(char *) * (i + 2));
	if (new == NULL)
		exit(1);
	i = 0;
	while (cmd->envp[i])
	{
		new[i] = ft_strdup((cmd->envp)[i]);
		free((cmd->envp)[i++]);
	}
	new_export(str, new, i);
	free(cmd->envp);
	cmd->envp = new;
	return (1);
}
