/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:20:05 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/17 19:51:37 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cmd_pipe_exec_begin(t_cmd *cmd, t_plst *tmp, int *fd, int **fds)
{
	// t_plst *heredoc_true_lst;
	int		pid;
	int		i = 0;

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

static int	cmd_pipe_exec_end(t_cmd *cmd, t_plst *tmp, int *fd, int **fds)
{
	// t_plst *heredoc_true_lst;
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
		if (tmp->heredoc_true) // heredoc이 있을때 읽는 파이프
		{
			dup2(tmp->heredoc_fd[0], STDIN_FILENO);
			// heredoc_true_lst = cmd->pipe_lst;
			// while (heredoc_true_lst)
			// {
			// 	write(2, "sibal\n", 6);
			// 	if (heredoc_true_lst->heredoc_true)
			// 	{
			// 		close(heredoc_true_lst->heredoc_fd[0]);
			// 		printf("\n%d",heredoc_true_lst->heredoc_fd[1]);
			// 		close(heredoc_true_lst->heredoc_fd[1]);
			// 	}
			// 	heredoc_true_lst = heredoc_true_lst->next;
			// }
		}
		if (tmp->rdr_true)
			dup2(tmp->file_fd, STDOUT_FILENO);
		if (ms_builtin_func(cmd, tmp))
			cmd_path_cat_exec(cmd, tmp);
		exit (EXIT_SUCCESS);
	}
	return (pid);
}

static int	cmd_pipe_exec_middle(t_cmd *cmd, t_plst *tmp, int *prev_fd, int *fd, int **fds)
{
	// t_plst *heredoc_true_lst;
	int		pid;
	int		i = 0;

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
		if (tmp->heredoc_true) // heredoc이 있을때 읽는 파이프
		{
			dup2(tmp->heredoc_fd[0], STDIN_FILENO);
			// heredoc_true_lst = cmd->pipe_lst;
			// while (heredoc_true_lst)
			// {
			// 	if (heredoc_true_lst->heredoc_true)
			// 	{
			// 		close(heredoc_true_lst->heredoc_fd[0]);
			// 		close(heredoc_true_lst->heredoc_fd[1]);
			// 	}
			// 	heredoc_true_lst = heredoc_true_lst->next;
			// }
		}
		if (tmp->rdr_true)
			dup2(tmp->file_fd, STDOUT_FILENO);
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
		int	sig = WTERMSIG(exit_sts);
		if (sig == SIGINT)
			ft_putstr_fd("\n", 1);
		else if (sig == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 1);
		cmd->sts.process_status = sig + 128;
	}
	ms_term_set(cmd, 0);
	tmp = cmd->pipe_lst;
	while (tmp)
	{
		if (tmp->rdr_true)
			close(tmp->file_fd);
		tmp = tmp->next;
	}
	i = 0;
	while (i < cmd->pipe_cnt)
		free(fds[i++]);
	free(fds);
	free(pid_idx);
}
