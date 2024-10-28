/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_line_token_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:33:09 by jinsecho          #+#    #+#             */
/*   Updated: 2024/10/28 20:17:35 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	quote_flag_len(char *line, char c, int *i, int *flag)
{
	int	k;

	k = 0;
	if (c == DOUBLE_QUOTE)
		*flag = 1;
	while (line[*i + k] != c && line[*i + k])
		k++;
	return (k);
}

char	*ms_line_tokenizing_quote(t_cmd *cmd, char *line, int *i)
{
	char	*ptr;
	int		k;
	int		quote_flag;

	quote_flag = 0;
	ptr = (char *)malloc(sizeof(char) * ft_strlen(line) + 1);
	if (ptr == NULL)
		exit (EXIT_FAILURE);
	if (line[(*i)++] == DOUBLE_QUOTE)
		k = quote_flag_len(line, DOUBLE_QUOTE, i, &quote_flag);
	else
		k = quote_flag_len(line, SINGLE_QUOTE, i, &quote_flag);
	ft_strlcpy(ptr, line + *i, k + 1);
	(*i) += k;
	if (line[*i] == 0)
	{
		free(ptr);
		return (NULL);
	}
	else
		(*i)++;
	if (quote_flag)
		ms_line_replace_env(cmd, &ptr, line);
	return (ptr);
}

char	*ms_line_tokenizing_str(t_cmd *cmd, char *line, int *i)
{
	int		k;
	char	*ptr;

	k = 0;
	ptr = (char *)malloc(sizeof(char) * ft_strlen(line) + 1);
	if (ptr == NULL)
		exit (EXIT_FAILURE);
	if (ft_strchr("|><", line[*i]))
		k++;
	else
	{
		while (ft_strchr(" |><\t\n", line[*i + k]) == NULL \
		&& line[*i + k] != DOUBLE_QUOTE \
		&& line[*i + k] != SINGLE_QUOTE && line[*i + k])
			k++;
	}
	ft_strlcpy(ptr, line + *i, k + 1);
	(*i) += k;
	ms_line_replace_env(cmd, &ptr, line);
	return (ptr);
}
