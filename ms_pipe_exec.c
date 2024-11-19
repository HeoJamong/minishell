/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:20:05 by jinsecho          #+#    #+#             */
/*   Updated: 2024/11/19 22:12:50 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cmd_pipe_exec_begin(t_cmd *cmd, t_plst *tmp, int *fd, int **fds)
{
	int	pid;
	int	i = 0;

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
		if (ms_builtin_func(cmd, tmp))
			cmd_path_cat_exec(cmd, tmp);
		exit (EXIT_SUCCESS);
	}
	return (pid);
}

static int	cmd_pipe_exec_end(t_cmd *cmd, t_plst *tmp, int *fd, int **fds)
{
	int	pid;
	int	i = 0;
	
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
		if (ms_builtin_func(cmd, tmp))
			cmd_path_cat_exec(cmd, tmp);
		exit (EXIT_SUCCESS);
	}
	return (pid);
}

static int	cmd_pipe_exec_middle(t_cmd *cmd, t_plst *tmp, int *prev_fd, int *fd, int **fds)
{
	int	pid;
	int	i = 0;

	pid = fork();
	if (pid == 0)
	{
		dup2(prev_fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		while (i < cmd->pipe_cnt)
		{
			close(fds[i][0]);
			close(fds[i][1]);
			i++;
		}
		if (ms_builtin_func(cmd, tmp))
			cmd_path_cat_exec(cmd, tmp);
		exit (EXIT_SUCCESS);	
	}
	return (pid);
}

void	cmd_pipe_exec(t_cmd *cmd, t_plst *tmp)
{
	int	**fds;
	int	*pid_idx;
	int	i;
	int	exit_sts;

	i = 0;
	fds = (int **)malloc(sizeof(int *) * cmd->pipe_cnt);
	while (i < cmd->pipe_cnt)
		fds[i++] = (int *)malloc(sizeof(int) * 2);
	pid_idx = (int *)malloc(sizeof(int) * (cmd->pipe_cnt + 1));
	i = 0;
	while (i < cmd->pipe_cnt)
	{
		if (pipe(fds[i]) == -1)
			exit (EXIT_FAILURE);
		i++;
	}
	i = 0;
	while (tmp)
	{
		if (tmp == cmd->pipe_lst)
		{	
			pid_idx[i] = cmd_pipe_exec_begin(cmd, tmp, fds[i], fds);
			close(fds[i][1]);
		}
		else if (tmp->next == NULL)
		{
			pid_idx[i] = cmd_pipe_exec_end(cmd, tmp, fds[i - 1], fds);
			close(fds[i - 1][0]);
		}
		else
		{
			pid_idx[i] = cmd_pipe_exec_middle(cmd, tmp, fds[i - 1], fds[i], fds);
			close(fds[i - 1][0]);
			close(fds[i][1]);
		}
		i++;
		tmp = tmp->next;
	}
	i = 0;
	while (i < cmd->pipe_cnt)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
	while (wait(&exit_sts) > 0);
	i = 0;
	while (i < cmd->pipe_cnt)
		free(fds[i++]);
	free(fds);
	free(pid_idx);
}