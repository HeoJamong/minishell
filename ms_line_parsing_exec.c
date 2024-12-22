/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_line_parsing_exec.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 21:09:00 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 21:25:45 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_cmd_free(t_cmd *cmd, t_plst *tmp)
{
	t_plst	*pipe_tmp;

	ft_line_split_free(cmd->line_split);
	while (tmp)
	{
		pipe_tmp = tmp;
		tmp = tmp->next;
		free(pipe_tmp->pipe_split);
		free(pipe_tmp->heredoc_fd);
		free(pipe_tmp);
	}
}

void	pipe_cnt(t_cmd *cmd, t_plst *tmp)
{
	int	i;

	i = 0;
	while (tmp->next)
	{
		i++;
		tmp = tmp->next;
	}
	cmd->pipe_cnt = i;
}

void	ms_line_parsing_exec(t_cmd *cmd)
{
	ms_line_tokenizer(cmd, cmd->line);
	if (ms_line_pipe_split(cmd))
		return (ft_line_split_free(cmd->line_split));
	if (ms_rdr_input_true(cmd, 0))
		return (ms_cmd_free(cmd, cmd->pipe_lst));
	if (ms_rdr_output_true(cmd, 0))
		return (ms_cmd_free(cmd, cmd->pipe_lst));
	pipe_cnt(cmd, cmd->pipe_lst);
	ms_exec(cmd);
	ms_cmd_free(cmd, cmd->pipe_lst);
}
