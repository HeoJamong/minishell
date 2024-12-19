/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:20:05 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/19 22:57:53 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	pipe_exec_begin(t_cmd *cmd, t_plst *tmp, int *fd, int **fds)
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

static int	pipe_exec_end(t_cmd *cmd, t_plst *tmp, int *fd, int **fds)
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

static int	pipe_exec_middle(\
			t_cmd *cmd, t_plst *tmp, int *prev_fd, int *fd, int **fds)
{
	int	i;
	int	pid;

	i = 0;
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

void	pipe_exec_set(t_cmd *cmd, int ***fds, int **pid_idx, int i)
{
	*fds = (int **)malloc(sizeof(int *) * cmd->pipe_cnt);
	if (*fds == NULL)
		exit (EXIT_FAILURE);
	while (i < cmd->pipe_cnt)
	{
		(*fds)[i] = (int *)malloc(sizeof(int) * 2);
		if ((*fds)[i++] == NULL)
			exit (EXIT_FAILURE);
	}
	*pid_idx = (int *)malloc(sizeof(int) * (cmd->pipe_cnt + 1));
	if (*pid_idx == NULL)
		exit (EXIT_FAILURE);
	i = 0;
	while (i < cmd->pipe_cnt)
		if (pipe((*fds)[i++]) == -1)
			exit (EXIT_FAILURE);
}

void	pipe_exec(t_cmd *cmd, t_plst *tmp, int **fds, int *pid_idx)
{
	int	i;

	i = 0;
	while (tmp)
	{
		if (tmp == cmd->pipe_lst)
		{	
			pid_idx[i] = pipe_exec_begin(cmd, tmp, fds[i], fds);
			close(fds[i][1]);
		}
		else if (tmp->next == NULL)
		{
			pid_idx[i] = pipe_exec_end(cmd, tmp, fds[i - 1], fds);
			close(fds[i - 1][0]);
		}
		else
		{
			pid_idx[i] = pipe_exec_middle(cmd, tmp, fds[i - 1], fds[i], fds);
			close(fds[i - 1][0]);
			close(fds[i][1]);
		}
		i++;
		tmp = tmp->next;
	}
}

void	pipe_close(t_cmd *cmd, int **fds, int i)
{
	t_plst	*tmp;

	i = 0;
	while (i < cmd->pipe_cnt)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
	tmp = cmd->pipe_lst;
	while (tmp)
	{
		if (tmp->heredoc_true)
		{
			close(tmp->heredoc_fd[0]);
			close(tmp->heredoc_fd[1]);
		}
		if (tmp->rdr_true)
			close(tmp->file_fd);
		tmp = tmp->next;
	}
}

void	pipe_exec_wait(t_cmd *cmd, int *pid_idx, int i)
{
	int	exit_sts;
	int	sig;

	i = 0;
	ms_term_set(cmd, 1);
	while (i < cmd->pipe_cnt + 1)
	{
		waitpid(pid_idx[i], &exit_sts, 0);
		i++;
	}
	if (WIFEXITED(exit_sts))
		cmd->sts.process_status = WEXITSTATUS(exit_sts);
	if (WIFSIGNALED(exit_sts))
	{
		sig = WTERMSIG(exit_sts);
		if (sig == SIGINT)
			ft_putstr_fd("^C\n", STDOUT_FILENO);
		else if (sig == SIGQUIT)
			ft_putstr_fd("^\\Quit (core dumped)\n", STDOUT_FILENO);
		cmd->sts.process_status = sig + 128;
	}
	ms_term_set(cmd, 0);
}

void	cmd_pipe_exec(t_cmd *cmd, t_plst *tmp)
{
	int	**fds;
	int	*pid_idx;
	int	i;

	pipe_exec_set(cmd, &fds, &pid_idx, 0);
	pipe_exec(cmd, tmp, fds, pid_idx);
	pipe_close(cmd, fds, 0);
	pipe_exec_wait(cmd, pid_idx, 0);
	i = 0;
	while (i < cmd->pipe_cnt)
		free(fds[i++]);
	free(fds);
	free(pid_idx);
}
