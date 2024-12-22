/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rdr_input2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 20:36:53 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/22 21:12:53 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_input_child_proc(t_cmd *cmd, int *fd, char *last_word)
{
	char	*str;
	char	*line;

	ms_term_reset(cmd, 1);
	close(fd[0]);
	line = ft_strdup("");
	while (1)
	{
		str = readline("> ");
		if (str == NULL)
			exit (EXIT_SUCCESS);
		if (ft_strlen(str) == ft_strlen(last_word) && \
			!ft_strncmp(str, last_word, ft_strlen(last_word)))
			break ;
		line = ft_strjoin(line, str);
		line = ft_strjoin(line, "\n");
		free(str);
	}
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	ft_putstr_fd(line, STDOUT_FILENO);
	exit (EXIT_SUCCESS);
}

void	heredoc_input(t_cmd *cmd, t_plst *tmp, char *last_word)
{
	int		*fd;
	int		pid;
	int		exit_sts;

	if (tmp->heredoc_fd)
		rdr_input_fd_init(tmp);
	fd = (int *)malloc(sizeof(int) * 2);
	if (fd == NULL)
		exit (EXIT_FAILURE);
	if (pipe(fd) == -1)
		exit (EXIT_FAILURE);
	tmp->heredoc_fd = fd;
	pid = (int)fork();
	if (pid == 0)
		heredoc_input_child_proc(cmd, fd, last_word);
	ms_term_set(cmd, 1);
	waitpid(pid, &exit_sts, 0);
	if (WIFEXITED(exit_sts))
		cmd->sts.process_status = WEXITSTATUS(exit_sts);
	ms_term_set(cmd, 0);
	close(fd[1]);
}

int	rdr_input_file_open(t_cmd *cmd, t_plst *tmp, char *filename)
{
	int		*fd;
	int		file_fd;

	if (tmp->heredoc_fd)
		rdr_input_fd_init(tmp);
	if (rdr_error_print(cmd, filename))
		return (1);
	fd = (int *)malloc(sizeof(int) * 2);
	if (fd == NULL)
		exit (EXIT_FAILURE);
	if (pipe(fd) == -1)
		exit (EXIT_FAILURE);
	tmp->heredoc_fd = fd;
	file_fd = open(filename, O_RDONLY);
	dup2(file_fd, fd[0]);
	return (0);
}

int	rdr_file_input(t_cmd *cmd, t_plst *tmp, int *i, int k)
{
	if (tmp->pipe_split[*i + 1] == NULL)
	{
		ms_error_print(0);
		tmp->rdr_input_file = 2;
		cmd->sts.process_status = 2;
		return (1);
	}
	tmp->heredoc_true = 1;
	if (rdr_input_file_open(cmd, tmp, tmp->pipe_split[*i + 1]))
		return (1);
	k = *i;
	k += 2;
	if (tmp->pipe_split[k] == NULL)
		tmp->pipe_split[*i] = NULL;
	else
	{
		while (tmp->pipe_split[k])
		{
			tmp->pipe_split[k - 2] = tmp->pipe_split[k];
			k++;
		}
		tmp->pipe_split[k - 2] = NULL;
	}
	return (0);
}

int	rdr_heredoc(t_cmd *cmd, t_plst *tmp, int *i, int k)
{
	if (tmp->pipe_split[*i + 1] == NULL)
	{
		ms_error_print(0);
		return (1);
	}
	tmp->heredoc_true = 1;
	heredoc_input(cmd, tmp, tmp->pipe_split[*i + 1]);
	k = *i;
	k += 2;
	if (tmp->pipe_split[k] == NULL)
		tmp->pipe_split[*i] = NULL;
	else
	{
		while (tmp->pipe_split[k])
		{
			tmp->pipe_split[k - 2] = tmp->pipe_split[k];
			k++;
		}
		tmp->pipe_split[k - 2] = NULL;
	}
	return (0);
}
