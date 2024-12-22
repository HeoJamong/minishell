/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_path_cat_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 20:41:03 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 20:44:47 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_dir_check(t_plst *tmp)
{
	void	*tmp_dir;

	tmp_dir = opendir(tmp->pipe_split[0]);
	if (tmp_dir)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
		exit (126);
	}
	free(tmp_dir);
}

void	exec_access_check(t_plst *tmp)
{
	if (access(tmp->pipe_split[0], X_OK) != 0)
	{
		if (access(tmp->pipe_split[0], F_OK) != 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
			ft_putendl_fd(": No such file or directory", STDERR_FILENO);
			exit (127);
		}
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
			ft_putendl_fd(": Permission denied", STDERR_FILENO);
			exit (126);
		}
	}
}

void	exec_path_find(t_cmd *cmd, t_plst *tmp, char **path)
{
	int	i;

	i = 0;
	while (cmd->envp[i])
	{
		*path = ft_envchr(cmd->envp[i++], "PATH");
		if (*path)
		{
			(*path) += 5;
			break ;
		}
	}
	if (*path == NULL)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		exit (127);
	}
}

void	exec_path_split(t_plst *tmp, char ***path_split, char *path)
{
	int	i;

	i = 0;
	*path_split = ft_split(path, ':');
	while ((*path_split)[i])
	{
		(*path_split)[i] = ft_strjoin((*path_split)[i], "/");
		(*path_split)[i] = ft_strjoin((*path_split)[i], tmp->pipe_split[0]);
		if (access((*path_split)[i], F_OK) == 0)
			break ;
		i++;
	}
	if ((*path_split)[i] == NULL)
	{
		ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit (127);
	}
	free(tmp->pipe_split[0]);
	tmp->pipe_split[0] = ft_strdup((*path_split)[i]);
	ft_line_split_free(*path_split);
}

void	cmd_path_cat_exec(t_cmd *cmd, t_plst *tmp)
{
	char	**path_split;
	char	*path;

	if (ft_strchr(tmp->pipe_split[0], '/'))
	{
		exec_dir_check(tmp);
		exec_access_check(tmp);
	}
	else
	{
		exec_path_find(cmd, tmp, &path);
		exec_path_split(tmp, &path_split, path);
	}
	if (execve(tmp->pipe_split[0], tmp->pipe_split, cmd->envp) == -1)
		exit (EXIT_FAILURE);
}
