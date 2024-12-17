/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rdr_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 20:36:53 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/17 19:51:48 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_input(t_cmd *cmd, t_plst *tmp, char *last_word)
{
	char	*str;
	char	*line;
	int		*fd;
	int		pid = 1;
	int		exit_sts;

	if (tmp->heredoc_fd)
	{
		close(tmp->heredoc_fd[0]);
		close(tmp->heredoc_fd[1]);
		free(tmp->heredoc_fd);
	}
	fd = (int *)malloc(sizeof(int) * 2);
	if (fd == NULL)
		exit (EXIT_FAILURE);
	if (pipe(fd) == -1)
		exit (EXIT_FAILURE);
	tmp->heredoc_fd = fd;
	pid = (int)fork();
	if (pid == 0)
	{
		ms_term_reset(cmd);
		close(fd[0]);
		line = ft_strdup("");
		while(1)
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
	ms_term_set(cmd, 1);
	waitpid(pid, &exit_sts, 0);
	if (WIFEXITED(exit_sts))
		cmd->sts.process_status = WEXITSTATUS(exit_sts);
	if (WIFSIGNALED(exit_sts))
	{
		int	sig = WTERMSIG(exit_sts);
		if (sig == SIGINT)
			ft_putstr_fd("\n", STDOUT_FILENO);
		else if (sig == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
		cmd->sts.process_status = sig + 128;
	}
	ms_term_set(cmd, 0);
	close(fd[1]);
}

static int	redirection_file_input(t_cmd *cmd, t_plst *tmp, char *filename)
{
	int		*fd;
	int		file_fd;

	if (tmp->heredoc_fd)
	{
		close(tmp->heredoc_fd[0]);
		close(tmp->heredoc_fd[1]);
		free(tmp->heredoc_fd);
		tmp->heredoc_fd = NULL;
	}
	if (access(filename, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		tmp->rdr_input_file = 1;
		cmd->sts.process_status = 1;
		return (1);
	}
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

int	ms_heredoc_true_input(t_cmd *cmd)
{
	t_plst	*tmp;
	int		i;
	int		k;

	tmp = cmd->pipe_lst;
	while (tmp)
	{
		i = 0;
		tmp->rdr_input_file = 0;
		tmp->heredoc_true = 0;
		tmp->heredoc_fd = NULL;
		while (tmp->pipe_split[i])
		{
			if (ft_strnstr(tmp->pipe_split[i], "<", 1) \
			&& ft_strlen(tmp->pipe_split[i]) == 1)
			{
				if (tmp->pipe_split[i + 1] == NULL)
				{
					ft_putstr_fd("minishell: ", STDERR_FILENO);
					ft_putendl_fd("syntax error near unexpected token `newline'", STDERR_FILENO);
					tmp->rdr_input_file = 2;
					cmd->sts.process_status = 2;
					return (1);
				}
				tmp->heredoc_true = 1;
				if (redirection_file_input(cmd, tmp, tmp->pipe_split[i + 1]))
					return (1);
				k = i;
				k += 2;
				if (tmp->pipe_split[k] == NULL)
					tmp->pipe_split[i] = NULL;
				else
				{
					while (tmp->pipe_split[k])
					{
						tmp->pipe_split[k - 2] = tmp->pipe_split[k];
						k++;
					}
					tmp->pipe_split[k - 2] = NULL;
				}
			}
			else if(ft_strnstr(tmp->pipe_split[i], "<<", 2) \
			&& ft_strlen(tmp->pipe_split[i]) == 2)
			{
				if (tmp->pipe_split[i + 1] == NULL)
				{
					ft_putstr_fd("minishell: ", STDERR_FILENO);
					ft_putendl_fd("syntax error near unexpected token `newline'", STDERR_FILENO);
					return (1);
				}
				tmp->heredoc_true = 1;
				heredoc_input(cmd, tmp, tmp->pipe_split[i + 1]);
				k = i;
				k += 2;
				if (tmp->pipe_split[k] == NULL)
					tmp->pipe_split[i] = NULL;
				else
				{
					while (tmp->pipe_split[k])
					{
						tmp->pipe_split[k - 2] = tmp->pipe_split[k];
						k++;
					}
					tmp->pipe_split[k - 2] = NULL;
				}
			}
			else
				i++;
		}
		tmp = tmp->next;
	}
	return (0);
}
