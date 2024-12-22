/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe_exec2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:20:05 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 20:23:01 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_exec_begin(t_cmd *cmd, t_plst *tmp, int *fd, int **fds)
{
	int	i;
	int	pid;

	i = 0;
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		while (i < cmd->pipe_cnt)
		{
			close(fds[i][0]);
			close(fds[i][1]);
			i++;
		}
		if (tmp->heredoc_true)
			dup2(tmp->heredoc_fd[0], STDIN_FILENO);
		if (tmp->rdr_true)
			dup2(tmp->file_fd, STDOUT_FILENO);
		if (ms_builtin_func(cmd, tmp))
			cmd_path_cat_exec(cmd, tmp);
		exit (EXIT_SUCCESS);
	}
	return (pid);
}

int	pipe_exec_end(t_cmd *cmd, t_plst *tmp, int *fd, int **fds)
{
	int	i;
	int	pid;

	i = 0;
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		while (i < cmd->pipe_cnt)
		{
			close(fds[i][0]);
			close(fds[i][1]);
			i++;
		}
		if (tmp->heredoc_true)
			dup2(tmp->heredoc_fd[0], STDIN_FILENO);
		if (tmp->rdr_true)
			dup2(tmp->file_fd, STDOUT_FILENO);
		if (ms_builtin_func(cmd, tmp))
			cmd_path_cat_exec(cmd, tmp);
		exit (EXIT_SUCCESS);
	}
	return (pid);
}

int	pipe_exec_middle(t_cmd *cmd, t_plst *tmp, t_pipefd *fd, int **fds)
{
	int	i;
	int	pid;

	i = 0;
	pid = fork();
	if (pid == 0)
	{
		dup2(fd->p[0], STDIN_FILENO);
		dup2(fd->c[1], STDOUT_FILENO);
		while (i < cmd->pipe_cnt)
		{
			close(fds[i][0]);
			close(fds[i][1]);
			i++;
		}
		if (tmp->heredoc_true)
			dup2(tmp->heredoc_fd[0], STDIN_FILENO);
		if (tmp->rdr_true)
			dup2(tmp->file_fd, STDOUT_FILENO);
		if (ms_builtin_func(cmd, tmp))
			cmd_path_cat_exec(cmd, tmp);
		exit (EXIT_SUCCESS);
	}
	return (pid);
}
