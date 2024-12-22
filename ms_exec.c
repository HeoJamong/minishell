/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 21:09:00 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/22 17:27:26 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_fd_dup(t_plst *tmp)
{
	if (tmp->heredoc_true)
		dup2(tmp->heredoc_fd[0], STDIN_FILENO);
	if (tmp->rdr_true)
		dup2(tmp->file_fd, STDOUT_FILENO);
}

void	exec_exit_sts(t_cmd *cmd, t_plst *tmp, int pid, int exit_sts)
{
	int	sig;

	if (ms_builtin_func(cmd, tmp))
	{
		pid = (int)fork();
		if (pid == 0)
		{
			ms_term_reset(cmd, 0);
			cmd_path_cat_exec(cmd, tmp);
		}
		ms_term_set(cmd, 1);
		waitpid(pid, &exit_sts, 0);
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
}

void	cmd_exec(t_cmd *cmd, t_plst *tmp)
{
	t_fd	fd;

	fd.input = dup(STDIN_FILENO);
	fd.output = dup(STDOUT_FILENO);
	exec_fd_dup(tmp);
	exec_exit_sts(cmd, tmp, 1, 0);
	close(tmp->file_fd);
	dup2(fd.input, STDIN_FILENO);
	dup2(fd.output, STDOUT_FILENO);
}

int	ms_exec(t_cmd *cmd)
{
	if (cmd->sts.pipe_true == 0)
		cmd_exec(cmd, cmd->pipe_lst);
	else
		cmd_pipe_exec(cmd, cmd->pipe_lst);
	return (0);
}
