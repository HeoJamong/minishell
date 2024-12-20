/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rdr_output2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:58:36 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 15:50:51 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirection_file_open(t_plst *tmp, char *filename)
{
	if (tmp->file_fd)
		close(tmp->file_fd);
	tmp->file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (tmp->file_fd == -1)
	{
		if (access(filename, X_OK) != 0)
		{
			if (access(filename, F_OK) != 0)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(filename, STDERR_FILENO);
				ft_putendl_fd(": No such file or directory", STDERR_FILENO);
			}
			else
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(filename, STDERR_FILENO);
				ft_putendl_fd(": Permission denied", STDERR_FILENO);
			}
		}
		return (1);
	}
	return (0);
}

static int	redirection_file_open_append(t_plst *tmp, char *filename)
{
	if (tmp->file_fd)
		close(tmp->file_fd);
	tmp->file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (tmp->file_fd == -1)
	{
		if (access(filename, X_OK) != 0)
		{
			if (access(filename, F_OK) != 0)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(filename, STDERR_FILENO);
				ft_putendl_fd(": No such file or directory", STDERR_FILENO);
			}
			else
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(filename, STDERR_FILENO);
				ft_putendl_fd(": Permission denied", STDERR_FILENO);
			}
		}
		return (1);
	}
	return (0);
}

int	rdr_file_output(t_plst *tmp, int *i, int k)
{
	if (tmp->pipe_split[*i + 1] == NULL)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd("syntax error near ", STDERR_FILENO);
		ft_putendl_fd("unexpected token `newline'", STDERR_FILENO);
		return (1);
	}
	tmp->rdr_true = 1;
	if (redirection_file_open(tmp, tmp->pipe_split[*i + 1]))
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

int	rdr_file_output_append(t_plst *tmp, int *i, int k)
{
	if (tmp->pipe_split[*i + 1] == NULL)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd("syntax error near ", STDERR_FILENO);
		ft_putendl_fd("unexpected token `newline'", STDERR_FILENO);
		return (1);
	}
	tmp->rdr_true = 1;
	if (redirection_file_open_append(tmp, tmp->pipe_split[*i + 1]))
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
