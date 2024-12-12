/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rdr_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 20:36:53 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/12 19:17:24 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_input(t_plst *tmp, char *last_word)
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
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		line = ft_strdup("");
		while(1)
		{
			str = readline("> ");
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
	else
	{
		close(fd[1]);
		waitpid(pid, &exit_sts, 0);
	}
}

static int	redirection_file_input(t_plst *tmp, char *filename)
{
	int		*fd;
	int		file_fd;

	if (tmp->heredoc_fd)
	{
		close(tmp->heredoc_fd[0]);
		close(tmp->heredoc_fd[1]);
		free(tmp->heredoc_fd);
	}
	if (access(filename, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
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
					return (1);
				}
				tmp->heredoc_true = 1;
				if (redirection_file_input(tmp, tmp->pipe_split[i + 1]))
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
				heredoc_input(tmp, tmp->pipe_split[i + 1]);
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