/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 21:09:00 by jinsecho          #+#    #+#             */
/*   Updated: 2024/09/05 13:57:22jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	replace_env_find(t_cmd *cmd, char **ptr, char *line, int *n)
{
	char	*tmp;
	int		i;

	i = 0;
	while (ft_isalnum(line[*n]) || line[*n] == '_')
		(*n)++;
	tmp = (char *)malloc(sizeof(char) * (*n));
	if (tmp == NULL)
		exit (EXIT_FAILURE);
	ft_strlcpy(tmp, line + 1, *n);
	while (cmd->envp[i])
	{
		if (ft_envchr(cmd->envp[i], tmp))
		{
			*ptr = ft_strdup(ft_strchr(cmd->envp[i], '=') + 1);
			free(tmp);
			return ;
		}
		i++;
	}
	*ptr = ft_strdup("");
	free(tmp);
}

static char	*ms_str_replace_env(t_cmd *cmd, char *line, int *n)
{
	char	*ptr;

	if (line[*n] == '?')
	{
		*n = 2;
		ptr = ft_itoa(0);
	}
	else if (ft_isalpha(line[*n]) || line[*n] == '_')
		replace_env_find(cmd, &ptr, line, n);
	else if (ft_isdigit(line[*n]))
	{
		*n = 2;
		ptr = ft_strdup("");
	}
	else
		ptr = ft_strdup("");
	return (ptr);
}

void	ms_line_replace_env(t_cmd *cmd, char **ptr, char *line)
{
	char	*env_tmp;
	char	*str_tmp;
	int		len;
	int		cnt;
	int		n;

	cnt = 0;
	n = 1;
	while ((*ptr)[cnt] != '$' && (*ptr)[cnt])
		cnt++;
	while ((*ptr)[cnt])
	{
		env_tmp = ms_str_replace_env(cmd, &(*ptr)[cnt], &n);
		if (n == 1)
			cnt++;
		else
		{
			len = ft_strlen(line) + ft_strlen(env_tmp);
			*ptr = ft_realloc(*ptr, len + 1);
			str_tmp = ft_strdup(&(*ptr)[cnt] + n);
			ft_strlcpy(&(*ptr)[cnt], env_tmp, ft_strlen(env_tmp) + 1);
			ft_strlcat(*ptr, str_tmp, len + 1);
			cnt += ft_strlen(env_tmp);
			free(str_tmp);
		}
		n = 1;
		while ((*ptr)[cnt] != '$' && (*ptr)[cnt])
			cnt++;
		free(env_tmp);
	}
}

int	quote_flag_len(char *line, char c, int *i, int *flag)
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
	char	*ptr;
	int		k;
	
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

void	ms_line_tokenizer(t_cmd *cmd, char *line)
{
	int		i = 0;
	int		line_i = 0;
	char	*tmp;

	cmd->line_split = (char **)malloc(sizeof(char *) * (ft_strlen(line) + 1));
	if (cmd->line_split == NULL)
		exit (EXIT_FAILURE);
	while (line[i])
	{
		if (line[i] == ' ')
		{
			while (line[i] == ' ')
				i++;
		}
		else if (line[i] == DOUBLE_QUOTE || line[i] == SINGLE_QUOTE)
		{
			cmd->line_split[line_i] = ms_line_tokenizing_quote(cmd, line, &i);
			while (ft_strchr(" |><", line[i]) == NULL && line[i])
			{
				if (line[i] == DOUBLE_QUOTE || line[i] == SINGLE_QUOTE)
				{
					tmp = ms_line_tokenizing_quote(cmd, line, &i);
					if (tmp == NULL)
					{
						cmd->line_split[++line_i] = NULL;
						i = 0;
						while (cmd->line_split[i])
							free(cmd->line_split[i++]);
						free(cmd->line_split);
						return ;
					}
				}
				else
					tmp = ms_line_tokenizing_str(cmd, line, &i);
				cmd->line_split[line_i] = ft_strjoin(cmd->line_split[line_i], tmp);
			}
			line_i++;
		}
		else if (line[i] == '|')
			cmd->line_split[line_i++] = ms_line_tokenizing_str(cmd, line, &i);
		else if (line[i] == '>')
		{
			cmd->line_split[line_i] = ms_line_tokenizing_str(cmd, line, &i);
			if (line[i] == '>')
			{
				tmp = ms_line_tokenizing_str(cmd, line, &i);
				cmd->line_split[line_i] = ft_strjoin(cmd->line_split[line_i], tmp);
				if (cmd->line_split[line_i] == NULL)
					exit (EXIT_FAILURE);
				free(tmp);
			}
			line_i++;
		}
		else if (line[i] == '<')
		{
			cmd->line_split[line_i] = ms_line_tokenizing_str(cmd, line, &i);
			if (line[i] == '<')
			{
				tmp = ms_line_tokenizing_str(cmd, line, &i);
				cmd->line_split[line_i] = ft_strjoin(cmd->line_split[line_i], tmp);
				if (cmd->line_split[line_i] == NULL)
					exit (EXIT_FAILURE);
				free(tmp);
			}
			line_i++;
		}
		else
		{
			cmd->line_split[line_i] = ms_line_tokenizing_str(cmd, line, &i);
			if (cmd->line_split[line_i] == NULL)
				exit (EXIT_FAILURE);
			while (ft_strchr(" |><", line[i]) == NULL && line[i])
			{
				if (line[i] == DOUBLE_QUOTE || line[i] == SINGLE_QUOTE)
				{
					tmp = ms_line_tokenizing_quote(cmd, line, &i);
					if (tmp == NULL)
					{
						cmd->line_split[++line_i] = NULL;
						i = 0;
						while (cmd->line_split[i])
							free(cmd->line_split[i++]);
						free(cmd->line_split);
						return ;
					}
				}
				else
					tmp = ms_line_tokenizing_str(cmd, line, &i);
				cmd->line_split[line_i] = ft_strjoin(cmd->line_split[line_i], tmp);
				if (cmd->line_split[line_i] == NULL)
					exit (EXIT_FAILURE);
				free(tmp);
			}
			line_i++;
		}
	}
	cmd->line_split[line_i] = NULL;

	i = 0;
	while(cmd->line_split[i])
		printf("check:%s\n", cmd->line_split[i++]);
	
	i = 0;
	while (cmd->line_split[i])
		free(cmd->line_split[i++]);
	free(cmd->line_split);
}

void	ms_line_str_parsing(t_cmd *cmd)
{
	ms_line_tokenizer(cmd, cmd->line);
}

int	main(int ac, char **av, char **envp)
{
	t_cmd	cmd;

	(void)av;
	if (ac != 1)
		return (0);
	cmd.envp = envp;
	while (1)
	{
		ms_term_set(&cmd);
		cmd.line = readline("minishell$ ");
		if (cmd.line == NULL)
		{
			printf("exit\n");
			exit (EXIT_SUCCESS);
		}
		add_history(cmd.line);
		ms_term_reset(&cmd);
		ms_line_str_parsing(&cmd);
		free(cmd.line);
	}
	return (0);
}