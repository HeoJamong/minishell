/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:40:36 by jinsecho          #+#    #+#             */
/*   Updated: 2024/08/10 21:04:47by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_line_str_tokenizer(t_cmd *cmd, char *line)
{
	int		i = 0;
	int		k;
	int		m;
	int		line_i = 0;

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
			k = 0;
			m = 0;
			if (line[i] == 34)
			{
				while (line[i] == 34)
					i++;
				while (line[i + k] != 34 && line[i + k])
					k++;
			}
			else
			{
				while (line[i] == 39)
					i++;
				while (line[i + k] != 39 && line[i + k])
					k++;
			}
			k++;
			cmd->line_split[line_i] = (char *)malloc(sizeof(char) * ft_strlen(line) + 1);
			if (cmd->line_split[line_i] == NULL)
				exit (1);
			ft_strlcpy(cmd->line_split[line_i], line + i, k);
			i += k;
			if (line[i] == 34)
			{
				while (line[i] != 34)
					i++;
			}
			else if (line[i] == 39)
			{
				while (line[i] != 39)
					i++;
			}
			if (!(line[i] == 39 || line[i] == 34) && line[i] != ' ' && line[i])
			{
				while (line[i + m] != ' ' && line[i + m])
					m++;
				m++;
				char	*tmp = (char *)malloc(sizeof(char) * m);
				if (tmp == NULL)
					exit (1);
				ft_strlcpy(tmp, line + i, m);
				ft_strlcat(cmd->line_split[line_i], tmp, k + m);
				free(tmp);
			}
			i += m;
			line_i++;
		}
		else if (line[i++] == '|')
		{
			cmd->line_split[line_i] = (char *)malloc(sizeof(char) * 2);
			ft_strlcpy(cmd->line_split[line_i], "|", 2);
			line_i++;
		}
		// else if (line[i] == '>' || line[i] == '<')
		// {
		// 	cmd->line_split[line_i] = (char *)malloc(sizeof(char) * 2);
		// 	if (line[i++] == '>')
		// 		ft_strlcpy(cmd->line_split[line_i], ">", 2);
		// 	else
		// 		ft_strlcpy(cmd->line_split[line_i], "<", 2);
		// 	line_i++;
		// }
		// else if (line[i] == '>>' || line[i] == '<<')
		// {
		// 	cmd->line_split[line_i] = (char *)malloc(sizeof(char) * 3);
		// 	if (line[i] == '>>')
		// 		ft_strlcpy(cmd->line_split[line_i], ">>", 3);
		// 	else
		// 		ft_strlcpy(cmd->line_split[line_i], "<<", 3);
		// 	line_i++;
		// 	i += 2;
		// }
		else
		{
			while (line[i + k] != 39 && line[i + k])
				k++;
			i++;
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
	ms_line_str_tokenizer(cmd, ft_strtrim(cmd->line, " "));
	// if (ft_strnstr(cmd->line_split[0], "echo", 4) && ft_strlen(cmd->line_split[0]) == 4)
	// {
	// 	if (ft_strnstr(cmd->line_split[1], "-n", 2) && ft_strlen(cmd->line_split[1]) == 2)
	// 	{
	// 		;
	// 	}
	// }
	// else if (ft_strnstr(cmd->line_split[0], "cd", 2) && ft_strlen(cmd->line_split[0]) == 2)
	// {
		
	// }
	// else if (ft_strnstr(cmd->line_split[0], "pwd", 3) && ft_strlen(cmd->line_split[0]) == 3)
	// {

	// }
	// else if (ft_strnstr(cmd->line_split[0], "exit", 4) && ft_strlen(cmd->line_split[0]) == 4)
	// {
	// 	printf("exit\n");
	// 	if (cmd->line_split[1])
	// 		printf("minishell: exit: %s: numeric argument required\n", cmd->line_split[1]);
	// 	exit (0);
	// }
}

int	main(void)
{
	t_cmd	cmd;

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