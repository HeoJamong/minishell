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

int	ms_line_tokenizer(t_cmd *cmd, char *line)
{
	int		i = 0;
	int		line_i = 0;
	char	*tmp;

	if (*line == 0)
		return (-1);
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
						return (-1);
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
						return (-1);
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
	cmd->line_i = line_i;

	i = 0;
	while(cmd->line_split[i])
		printf("check:%s\n", cmd->line_split[i++]);
	return (0);
}

void	ms_builtin_func(t_cmd *cmd)
{
	int		dir;
	char	*tmp;
	char	*command;

	command = cmd->line_split[0];
	if (ft_strnstr(command, "cd", 2) && ft_strlen(command) == 2)
	{
		if (cmd->line_i > 2)
			printf("minishell: cd: too many arguments\n");
		else if (cmd->line_i == 1)
			chdir(getenv("HOME"));
		else if (ft_strlen(cmd->line_split[1]) == 0)
			chdir(getenv("HOME"));
		else if (strncmp("~", cmd->line_split[1], 1) == 0)
		{
			if (ft_strlen(cmd->line_split[1]) == 1)
				chdir(getenv("HOME"));
			else if (cmd->line_split[1][1] != '/')
			{
				tmp = cmd->line_split[1];
				dir = chdir(tmp);
				if (dir)
					printf("minishell: cd: %s: No such file or directory\n", tmp);
			}
			else
			{
				tmp = ft_strjoin(ft_strdup(getenv("HOME")), &(cmd->line_split[1][1]));
				dir = chdir(tmp);
				if (dir)
					printf("minishell: cd: %s: No such file or directory\n", tmp);
				free(tmp);
			}
		}
		else if (strncmp("/", cmd->line_split[1], 1) == 0)
		{
			tmp = cmd->line_split[1];
			dir = chdir(tmp);
			if (dir)
				printf("minishell: cd: %s: No such file or directory\n", tmp);
		}
		else
		{
			tmp = ft_strjoin(getcwd(NULL, 0), "/");
			tmp = ft_strjoin(tmp, cmd->line_split[1]);
			dir = chdir(tmp);
			if (dir)
				printf("minishell: cd: %s: No such file or directory\n", cmd->line_split[1]);
			free(tmp);
		}
	}
	else if (ft_strnstr(command, "pwd", 3) && ft_strlen(command) == 3)
	{
		char	*currdir;

		currdir = getcwd(NULL, 0);// goinfre의 경우는 아마 심볼릭 링크로 연결되어 있어서, 표현된 디렉토리에 차이를 보임
		if (currdir == NULL)		// 이부분 수정을 할 지 고민중
			exit (EXIT_FAILURE);
		printf("%s\n", currdir);
		free(currdir);
	}
	else if (ft_strnstr(command, "echo", 4) && ft_strlen(command) == 4)
	{
		int	i = 1;

		if (ft_strnstr(cmd->line_split[1], "-n", 2) && ft_strlen(cmd->line_split[1]) == 2)
		{
			i = 2;
			while (i + 1 < cmd->line_i)
				printf("%s ", cmd->line_split[i++]);
			printf("%s", cmd->line_split[i]);
		}
		else
		{
			while (i + 1 < cmd->line_i)
				printf("%s ", cmd->line_split[i++]);
			printf("%s\n", cmd->line_split[i]);
		}
	}
	else if (ft_strnstr(command, "export", 6) && ft_strlen(command) == 6)
	{
		if (ft_export(cmd->line_split[1], cmd) == 1)	
			printf("ok\n");
		print_env(cmd->envp);
	}
	else if (ft_strnstr(command, "unset", 5) && ft_strlen(command))
	{
		if (ft_unset(cmd->line_split[1], cmd) == 1)
			printf("ok\n");
		print_env(cmd->envp);
	}
	else if (ft_strnstr(command, "env",3) && ft_strlen(command) == 3)
		print_env(cmd->envp);
	else if (ft_strnstr(command, "exit", 4) && ft_strlen(command) == 4)
	{
		printf("exit\n");
		if (cmd->line_i == 1)
			exit(EXIT_SUCCESS);
		else
			printf("minishell: exit: too many arguments\n");
	}

}
int	ms_line_pipesplit(t_cmd *cmd)
{
	t_pipe_lst	*tmp;
	// t_pipe_lst	*pipe_tmp;
	int			i;
	int			k;

	i = 0;
	k = 0;
	cmd->pipe_lst = mini_lstnew();	// 일단 새로운 구조체 리스트 할당
	tmp = cmd->pipe_lst;
	tmp->line_pipesplit = (char **)malloc(sizeof(char *) * (cmd->line_i + 1));	//할당된 구조체에 2차원 배열 할당후 안에 들어갈 값 넣기
	if (tmp->line_pipesplit == NULL)
		exit (EXIT_FAILURE);
	while(i < cmd->line_i)
	{
		tmp->line_pipesplit[k++] = cmd->line_split[i++];
		if (cmd->line_split[i] == NULL)
			break ;
		if (ft_strnstr(cmd->line_split[i], "|", 1) && ft_strlen(cmd->line_split[i]) == 1)
		{
			tmp->line_pipesplit[k] = NULL;
			k = 0;
			mini_lstadd_back(&(cmd->pipe_lst), mini_lstnew());
			tmp = tmp->next;
			tmp->line_pipesplit = (char **)malloc(sizeof(char *) * cmd->line_i);
			if (tmp->line_pipesplit == NULL)
				exit (EXIT_FAILURE);
			i++;
			// if (ft_strnstr(cmd->line_split[i], "|", 1) && ft_strlen(cmd->line_split[i]) == 1)
			// {
			// 	tmp = cmd->pipe_lst;
			// 	while (tmp->next)
			// 	{
			// 		pipe_tmp = tmp;
			// 		tmp = tmp->next;
			// 		free(pipe_tmp->line_pipesplit);
			// 		free(pipe_tmp);
			// 	}
			// 	return (-1);
			// }
		}
		tmp->line_pipesplit[k] = NULL;
	}
	
	k = 0;
	tmp = cmd->pipe_lst;
	// while (tmp)
	// {
	// 	i = 0;
	// 	while (tmp->line_pipesplit[i])
	// 	{
	// 		printf("%d: %s\n", k, tmp->line_pipesplit[i]);
	// 		i++;
	// 	}
	// 	k++;
	// 	tmp = tmp->next;
	// }
	return (0);
}

void	ms_line_str_parsing(t_cmd *cmd)
{
	if (ms_line_tokenizer(cmd, cmd->line) == -1)
		return ;
	if (ms_line_pipesplit(cmd) == -1)
	{
		int	i = 0;
		while (cmd->line_split[i])
			free(cmd->line_split[i++]);
		free(cmd->line_split);
		return ;
	}
	ms_builtin_func(cmd);


	t_pipe_lst	*tmp;
	t_pipe_lst	*pipe_tmp;
	int			i = 0;

	tmp = cmd->pipe_lst;
	while (tmp->next)
	{
		free(tmp->line_pipesplit);
		pipe_tmp = tmp;
		tmp = tmp->next;
		free(pipe_tmp);
	}
	while (cmd->line_split[i])
		free(cmd->line_split[i++]);
	free(cmd->line_split);
}

int	main(int ac, char **av, char **envp)
{
	t_cmd		cmd;

	(void)av;
	if (ac != 1)
		return (0);
	cmd.envp = set_env(envp);
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