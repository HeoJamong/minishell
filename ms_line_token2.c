/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_line_token2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:46:02 by jinsecho          #+#    #+#             */
/*   Updated: 2024/10/29 15:46:25 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	line_token_redirect_join(\
			t_cmd *cmd, char *line, int *line_i, int *i)
{
	char	*tmp;

	tmp = ms_line_tokenizing_str(cmd, line, i);
	cmd->line_split[*line_i] = ft_strjoin(cmd->line_split[*line_i], tmp);
	if (cmd->line_split[*line_i] == NULL)
		exit (EXIT_FAILURE);
	free(tmp);
}

void	line_token_redirect(t_cmd *cmd, char *line, int *line_i, int *i)
{
	if (line[*i] == '>')
	{
		cmd->line_split[*line_i] = ms_line_tokenizing_str(cmd, line, i);
		if (line[*i] == '>')
			line_token_redirect_join(cmd, line, line_i, i);
	}
	else
	{
		cmd->line_split[*line_i] = ms_line_tokenizing_str(cmd, line, i);
		if (line[*i] == '<')
			line_token_redirect_join(cmd, line, line_i, i);
	}
	(*line_i)++;
}

void	line_token_str(t_cmd *cmd, char *line, int *line_i, int *i)
{
	char	*tmp;

	cmd->line_split[*line_i] = ms_line_tokenizing_str(cmd, line, i);
	if (cmd->line_split[*line_i] == NULL)
		exit (EXIT_FAILURE);
	while (ft_strchr(" |><", line[*i]) == NULL && line[*i])
	{
		if (line[*i] == DOUBLE_QUOTE || line[*i] == SINGLE_QUOTE)
		{
			tmp = ms_line_tokenizing_quote(cmd, line, i);
			if (tmp == NULL)
			{
				cmd->line_split[++(*line_i)] = NULL;
				line_split_free(cmd);
				return ;
			}
		}
		else
			tmp = ms_line_tokenizing_str(cmd, line, i);
		cmd->line_split[*line_i] = ft_strjoin(cmd->line_split[*line_i], tmp);
		free(tmp);
	}
	(*line_i)++;
}