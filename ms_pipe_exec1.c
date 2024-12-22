/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe_exec1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:20:05 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 20:13:02 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
