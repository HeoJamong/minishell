/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipe_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:56:21 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/19 16:02:45 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lst_free(t_cmd *cmd, t_plst **tmp)
{
	t_plst	*pipe_tmp;

	*tmp = cmd->pipe_lst;
	while (*tmp)
	{
		pipe_tmp = *tmp;
		*tmp = (*tmp)->next;
		free(pipe_tmp->pipe_split);
		free(pipe_tmp);
	}
	return (1);
}

static int	line_pipe_split_find(t_cmd *cmd, t_plst **tmp, int *i, int *k)
{
	if (cmd->line_split[*i + 1] == NULL)
		return (lst_free(cmd, tmp));
	(*tmp)->pipe_split[*k] = NULL;
	*k = 0;
	ms_lstadd_back(&(cmd->pipe_lst), ms_lstnew());
	*tmp = (*tmp)->next;
	(*tmp)->pipe_split = (char **)malloc(sizeof(char *) * cmd->line_i);
	if ((*tmp)->pipe_split == NULL)
		exit (EXIT_FAILURE);
	(*i)++;
	if (ft_strnstr(cmd->line_split[*i], "|", 1) && \
		ft_strlen(cmd->line_split[*i]) == 1)
		return (lst_free(cmd, tmp));
	return (0);
}

void	ms_idx_init(t_tmp *idx)
{
	idx->i = 0;
	idx->k = 0;
}

int	line_pipe_split_set(t_cmd *cmd, t_plst **tmp)
{
	if (cmd->line_split[0] == NULL || \
		(ft_strnstr(cmd->line_split[0], "|", 1) && \
		ft_strlen(cmd->line_split[0]) == 1))
		return (1);
	cmd->pipe_lst = ms_lstnew();
	*tmp = cmd->pipe_lst;
	(*tmp)->pipe_split = (char **)malloc(sizeof(char *) * (cmd->line_i + 1));
	if ((*tmp)->pipe_split == NULL)
		exit (EXIT_FAILURE);
	return (0);
}

int	ms_line_pipe_split(t_cmd *cmd)
{
	t_plst	*tmp;
	t_tmp	idx;

	ms_idx_init(&idx);
	if (line_pipe_split_set(cmd, &tmp))
		return (1);
	while (idx.i < cmd->line_i)
	{
		tmp->pipe_split[(idx.k)++] = cmd->line_split[(idx.i)++];
		if (cmd->line_split[idx.i] == NULL)
		{
			tmp->pipe_split[idx.k] = NULL;
			break ;
		}
		if (ft_strnstr(cmd->line_split[idx.i], "|", 1) && \
			ft_strlen(cmd->line_split[idx.i]) == 1)
		{
			cmd->sts.pipe_true = 1;
			if (line_pipe_split_find(cmd, &tmp, &idx.i, &idx.k))
				return (1);
		}
		tmp->pipe_split[idx.k] = NULL;
	}
	return (0);
}
