/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe_exec3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:20:05 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 20:15:02 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pipe_fd_input(t_pipefd *fd, int **fds, int i)
{
	fd->p = fds[i - 1];
	fd->c = fds[i];
}

static void	pipe_fd_close(int **fds, int i)
{
	close(fds[i - 1][0]);
	close(fds[i][1]);
}

void	pipe_exec(t_cmd *cmd, t_plst *tmp, int **fds, int *pid_idx)
{
	t_pipefd	fd;
	int			i;

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
			pipe_fd_input(&fd, fds, i);
			pid_idx[i] = pipe_exec_middle(cmd, tmp, &fd, fds);
			pipe_fd_close(fds, i);
		}
		i++;
		tmp = tmp->next;
	}
}
