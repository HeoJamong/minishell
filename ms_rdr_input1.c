/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rdr_input2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 20:36:53 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/19 21:53:51 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_error_print(int i)
{
	if (i == 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd("syntax error near ", STDERR_FILENO);
		ft_putendl_fd("unexpected token `newline'", STDERR_FILENO);
	}
}

void	rdr_input_fd_init(t_plst *tmp)
{
	close(tmp->heredoc_fd[0]);
	close(tmp->heredoc_fd[1]);
	free(tmp->heredoc_fd);
	tmp->heredoc_fd = NULL;
}

void	rdr_input_var_init(int *i, t_plst *tmp)
{
	*i = 0;
	tmp->rdr_input_file = 0;
	tmp->heredoc_true = 0;
	tmp->heredoc_fd = NULL;
}

int	ms_rdr_input_true(t_cmd *cmd)
{
	t_plst	*tmp;
	int		i;

	tmp = cmd->pipe_lst;
	while (tmp)
	{
		rdr_input_var_init(&i, tmp);
		while (tmp->pipe_split[i])
		{
			if (ft_strnstr(tmp->pipe_split[i], "<", 1) && ft_strlen(tmp->pipe_split[i]) == 1)
			{
				if (rdr_file_input(cmd, tmp, &i, 0))
					return (1);
			}
			else if (ft_strnstr(tmp->pipe_split[i], "<<", 2) && ft_strlen(tmp->pipe_split[i]) == 2)
			{
				if (rdr_heredoc(cmd, tmp, &i, 0))
					return (1);
			}
			else
				i++;
		}
		tmp = tmp->next;
	}
	return (0);
}
