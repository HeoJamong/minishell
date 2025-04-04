/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_line_token1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:56:11 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/26 22:56:13 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	line_token_var_init(t_cmd *cmd, int *i, int *line_i)
{
	cmd->sts.pipe_true = 0;
	*i = 0;
	*line_i = 0;
}

void	ms_line_split_free(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->line_split[i])
	{
		free(cmd->line_split[i]);
		cmd->line_split[i] = NULL;
		i++;
	}
	free(cmd->line_split);
	cmd->line_split = NULL;
}

void	line_token_quote(t_cmd *cmd, char *line, int *line_i, int *i)
{
	char	*tmp;

	cmd->line_split[*line_i] = ms_line_tokenizing_quote(cmd, line, i);
	while (ft_strchr(" |><", line[*i]) == NULL && line[*i])
	{
		if (line[*i] == DOUBLE_QUOTE || line[*i] == SINGLE_QUOTE)
		{
			tmp = ms_line_tokenizing_quote(cmd, line, i);
			if (tmp == NULL)
			{
				cmd->line_split[++(*line_i)] = NULL;
				ms_line_split_free(cmd);
				return ;
			}
		}
		else
			tmp = ms_line_tokenizing_str(cmd, line, i);
		cmd->line_split[*line_i] = ft_strjoin(cmd->line_split[*line_i], tmp);
		free(tmp);
	}
	if (ft_strlen(cmd->line_split[*line_i]) == 0)
		free(cmd->line_split[*line_i]);
	else
		(*line_i)++;
}

int	line_token_input(t_cmd *cmd, char *line, int *i, int *line_i)
{
	if (line[*i] == ' ')
		while (line[*i] == ' ')
			(*i)++;
	else if (line[*i] == DOUBLE_QUOTE || line[*i] == SINGLE_QUOTE)
	{
		line_token_quote(cmd, line, line_i, i);
		if (cmd->line_split == NULL)
			return (1);
	}
	else if (line[*i] == '|')
		cmd->line_split[(*line_i)++] = ms_line_tokenizing_str(cmd, line, i);
	else if (line[*i] == '>' || line[*i] == '<')
		line_token_redirect(cmd, line, line_i, i);
	else
	{
		line_token_str(cmd, line, line_i, i);
		if (cmd->line_split == NULL)
			return (1);
	}
	return (0);
}

int	ms_line_tokenizer(t_cmd *cmd, char *line, int i, int line_i)
{
	line_token_var_init(cmd, &i, &line_i);
	cmd->line_split = (char **)malloc(sizeof(char *) * (ft_strlen(line) + 1));
	if (cmd->line_split == NULL)
		exit (EXIT_FAILURE);
	while (line[i])
		if (line_token_input(cmd, line, &i, &line_i))
			return (1);
	cmd->line_split[line_i] = NULL;
	cmd->line_i = line_i;
	return (0);
}
