/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 21:09:00 by jinsecho          #+#    #+#             */
/*   Updated: 2024/09/03 19:25:47 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ms_line_replace_env(t_cmd *cmd, char *line, int *n)
{
	char	*ptr;
	char	*tmp;
	int		i = 1;

	if (line[i] == '?')
	{
		*n = 2;
		ptr = ft_itoa(0);
	}
	else if (ft_isalpha(line[i]) || line[i] == '_')
	{
		while (ft_isalnum(line[i]) || line[i] == '_')
		{
			(*n)++;
			i++;
		}
		tmp = (char *)malloc(sizeof(char) * i);
		if (tmp == NULL)
			exit (EXIT_FAILURE);
		ft_strlcpy(tmp, line + 1, i);
		i = 0;
		while (cmd->envp[i])
		{
			if (ft_envchr(cmd->envp[i], tmp))
			{
				ptr = ft_strdup(ft_strchr(cmd->envp[i], '=') + 1);
				free(tmp);
				return (ptr);
			}
			i++;
		}
		ptr = ft_strdup("");
		free(tmp);
	}
	else if (ft_isdigit(line[i]))
	{
		*n = 2;
		ptr = ft_strdup("");
	}
	else
		ptr = ft_strdup("");
	return (ptr);
}

char	*ms_line_tokenizing_quote(t_cmd *cmd, char *line, int *i)
{
	char	*ptr;
	char	*tmp;
	char	*env_tmp = NULL;
	char	*str_tmp;
	// char	*ptr_tmp;
	int		k = 0;
	int		n = 1;
	int		quote_flag = 0;

	ptr = (char *)malloc(sizeof(char) * ft_strlen(line) + 1);
	if (ptr == NULL)
		exit (1);
	if (line[(*i)++] == 34)
	{
		quote_flag = 1;
		while (line[*i + k] != 34 && line[*i + k])
			k++;
	}
	else
	{
		while (line[*i + k] != 39 && line[*i + k])
			k++;
	}
	ft_strlcpy(ptr, line + *i, k + 1);
	(*i) += k;
	if (line[*i] == 34 || line[*i] == 39)
		(*i)++;
	if (quote_flag)
	{
		env_tmp = ft_strchr(ptr, '$');
		while (env_tmp != NULL)
		{
			tmp = ms_line_replace_env(cmd, env_tmp, &n);
			if (n == 1)
			{
				free(tmp);
				break ;
			}
			else
			{
				ptr = ft_realloc(ptr, ft_strlen(line) + ft_strlen(tmp) + 1);
				env_tmp = ft_strchr(ptr, '$');
				str_tmp = ft_strdup(env_tmp + n);
				ft_strlcpy(env_tmp, tmp, ft_strlen(tmp) + 1);
				ptr = ft_strjoin(ptr, str_tmp);
				env_tmp = ft_strchr(ptr, '$');
				n = 1;
				free(tmp);
				free(str_tmp);
			}
		}
	}
	return (ptr);
}

char	*ms_line_tokenizing_str(char *line, int *i)
{
	char	*ptr;
	int		k = 0;
	
	ptr = (char *)malloc(sizeof(char) * ft_strlen(line) + 1);
	if (ptr == NULL)
		exit (1);
	if (ft_strchr("|><", line[*i]))
		k++;
	else
	{
		while (ft_strchr(" |><\t\n", line[*i + k]) == NULL \
		&& line[*i + k] != 34 && line[*i + k] != 39 && line[*i + k])
		k++;
	}
	ft_strlcpy(ptr, line + *i, k + 1);
	(*i) += k;
	return (ptr);
}

void	ms_line_tokenizer(t_cmd *cmd, char *line)
{
	int		i = 0;
	int		line_i = 0;
	char	*tmp;

	if (line == NULL)
		exit (1);
	// 34: "", 39: ' 아스키
	int	line_len = ft_strlen(line);
	cmd->line_split = (char **)malloc(sizeof(char *) * line_len);
	if (cmd->line_split == NULL)
		exit (1);
	while (line[i])
	{
		if (line[i] == ' ')
		{
			while (line[i] == ' ')
				i++;
		}
		else if (line[i] == 34 || line[i] == 39)
		{
			cmd->line_split[line_i] = ms_line_tokenizing_quote(cmd, line, &i);
			while (ft_strchr(" |><", line[i]) == NULL && line[i])
			{
				if (line[i] == 34 || line[i] == 39)
					tmp = ms_line_tokenizing_quote(cmd, line, &i);
				else
					tmp = ms_line_tokenizing_str(line, &i);
				cmd->line_split[line_i] = ft_strjoin(cmd->line_split[line_i], tmp);
				if (cmd->line_split[line_i] == NULL)
					exit (1);
				free(tmp);
			}
			line_i++;
		}
		else if (line[i] == '|')
			cmd->line_split[line_i++] = ms_line_tokenizing_str(line, &i);
		else if (line[i] == '>')
		{
			cmd->line_split[line_i] = ms_line_tokenizing_str(line, &i);
			if (line[i] == '>')
			{
				tmp = ms_line_tokenizing_str(line, &i);
				cmd->line_split[line_i] = ft_strjoin(cmd->line_split[line_i], tmp);
				if (cmd->line_split[line_i] == NULL)
					exit (1);
				free(tmp);
			}
			line_i++;
		}
		else if (line[i] == '<')
		{
			cmd->line_split[line_i] = ms_line_tokenizing_str(line, &i);
			if (line[i] == '<')
			{
				tmp = ms_line_tokenizing_str(line, &i);
				cmd->line_split[line_i] = ft_strjoin(cmd->line_split[line_i], tmp);
				if (cmd->line_split[line_i] == NULL)
					exit (1);
				free(tmp);
			}
			line_i++;
		}
		else
		{
			cmd->line_split[line_i] = ms_line_tokenizing_str(line, &i);
			if (cmd->line_split[line_i] == NULL)
				exit (1);
			while (ft_strchr(" |><", line[i]) == NULL && line[i])
			{
				if (line[i] == 34 || line[i] == 39)
					tmp = ms_line_tokenizing_quote(cmd, line, &i);
				else
					tmp = ms_line_tokenizing_str(line, &i);
				cmd->line_split[line_i] = ft_strjoin(cmd->line_split[line_i], tmp);
				if (cmd->line_split[line_i] == NULL)
					exit (1);
				free(tmp);
			}
			line_i++;
		}
	}
	cmd->line_split[line_i] = NULL;

	i = 0;
	while(cmd->line_split[i])
		printf("check: %s\n", cmd->line_split[i++]);
	
	
	i = 0;
	while (cmd->line_split[i])
		free(cmd->line_split[i++]);
	free(cmd->line_split);
	free(line);
}

void	ms_line_str_parsing(t_cmd *cmd)
{
	ms_line_tokenizer(cmd, ft_strtrim(cmd->line, " "));
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
			exit (0);
		}
		add_history(cmd.line);
		ms_term_reset(&cmd);
		ms_line_str_parsing(&cmd);
		free(cmd.line);
	}
	return (0);
}