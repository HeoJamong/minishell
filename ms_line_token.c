/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_line_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:56:11 by jinsecho          #+#    #+#             */
/*   Updated: 2024/10/28 20:01:00 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
				i = 0;
				while (cmd->line_split[*i])
					free(cmd->line_split[(*i)++]);
				free(cmd->line_split);
				return ;
			}
		}
		else
			tmp = ms_line_tokenizing_str(cmd, line, i);
		cmd->line_split[*line_i] = ft_strjoin(cmd->line_split[*line_i], tmp);
	}
	(*line_i)++;
}

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

void	ms_line_tokenizer(t_cmd *cmd, char *line)
{
	int	i;
	int	line_i;

	i = 0;
	line_i = 0;
	cmd->line_split = (char **)malloc(sizeof(char *) * (ft_strlen(line) + 1));
	if (cmd->line_split == NULL)
		exit (EXIT_FAILURE);
	while (line[i])
	{
		if (line[i] == ' ')
			while (line[i] == ' ')
				i++;
		else if (line[i] == DOUBLE_QUOTE || line[i] == SINGLE_QUOTE)
			line_token_quote(cmd, line, &line_i, &i);
		else if (line[i] == '|')
			cmd->line_split[line_i++] = ms_line_tokenizing_str(cmd, line, &i);
		else if (line[i] == '>' || line[i] == '<')
			line_token_redirect(cmd, line, &line_i, &i);
		else
			line_token_str(cmd, line, &line_i, &i);
	}
	cmd->line_split[line_i] = NULL;
	cmd->line_i = line_i;
}