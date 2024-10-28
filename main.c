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

int	exit_num_isdigit(char *num, int *flag)
{
	int	i;
	i = 0;
	
	if (num[i] == '-')
	{
		*flag = 1;
		i++;
	}
	while (num[i])
	{
		if (ft_isdigit(num[i]) == 0)
				return (0);
		i++;
	}
	return (1);
}

int	max_exit_num_check(char num, int i)
{
	const char	*max_exit_num = "9223372036854775807";

	if (num > max_exit_num[i])
		return (1);
	return (0);
}

int	min_exit_num_check(char num, int i)
{
	const char	*min_exit_num = "-9223372036854775808";
	if (num > min_exit_num[i])
		return (1);
	return (0);
}

void	ms_builtin_func(t_cmd *cmd)
{
	int		dir;
	char	*tmp;

	if (ft_strnstr(cmd->line_split[0], "cd", 2) && ft_strlen(cmd->line_split[0]) == 2)
	{
		if (cmd->line_i > 2)
			printf("minishell: cd: too many arguments\n");
		else if (cmd->line_i == 1)
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
	else if (ft_strnstr(cmd->line_split[0], "pwd", 3) && ft_strlen(cmd->line_split[0]) == 3)
	{
		char	*currdir;

		currdir = getcwd(NULL, 0);// goinfre의 경우는 아마 심볼릭 링크로 연결되어 있어서, 표현된 디렉토리에 차이를 보임
		if (currdir == NULL)		// 이부분 수정을 할 지 고민중
			exit (EXIT_FAILURE);
		printf("%s\n", currdir);
		free(currdir);
	}
	else if (ft_strnstr(cmd->line_split[0], "echo", 4) && ft_strlen(cmd->line_split[0]) == 4)
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
	else if (ft_strnstr(cmd->line_split[0], "exit", 4) && ft_strlen(cmd->line_split[0]) == 4)
	{
		printf("exit\n");
		int i = 0;
		int	flag;
		int	exit_num_check;

		flag = 0;
		if (cmd->line_i == 1)
			exit(EXIT_SUCCESS);
		else if (cmd->line_i == 2)
		{
			printf("호출 됐나요?\n");
			if (exit_num_isdigit(cmd->line_split[1], &flag) == 0)
				printf("숫자 아님\n");
			else
			{
				if (flag == 0 && strlen(cmd->line_split[1]) == 19)
				{
					while (cmd->line_split[1][i])
					{
						exit_num_check = max_exit_num_check(cmd->line_split[1][i], i);
						if (exit_num_check == 1)
							break;
						i++;
					}
					if (exit_num_check == 0)
						printf("exit: %lld\n", ft_atol(cmd->line_split[1]));
					else
						printf("올바른 값이 아님\n");
				}
				else if (flag == 1 && strlen(cmd->line_split[1]) == 20)
				{
					while (cmd->line_split[1][i])
					{
						exit_num_check = min_exit_num_check(cmd->line_split[1][i], i);
						if (exit_num_check == 1)
							break;
						i++;
					}
					if (exit_num_check == 0)
						printf("exit: %lld\n", ft_atol(cmd->line_split[1]));
					else
						printf("올바른 값이 아님\n");
				}
				else if ((flag == 0 && strlen(cmd->line_split[1]) < 19 && strlen(cmd->line_split[1]) > 0) \
				|| (flag == 1 && strlen(cmd->line_split[1]) < 20 && strlen(cmd->line_split[1]) > 0))
					printf("exit: %lld\n",ft_atol(cmd->line_split[1]));
			}
		}
		else
			printf("minishell: exit: too many arguments\n");
	}
	else if (ft_strnstr(cmd->line_split[0], "export", 6) && ft_strlen(cmd->line_split[0]) == 6)
	{
		if (ft_export(cmd->line_split[1], cmd) == 1)	
			printf("ok\n");
		print_env(cmd->envp);
	}
	else if (ft_strnstr(cmd->line_split[0], "unset", 5) && ft_strlen(cmd->line_split[0]))
	{
		if (ft_unset(cmd->line_split[1], cmd) == 1)
			printf("ok\n");
		print_env(cmd->envp);
	}
	else if (ft_strnstr(cmd->line_split[0], "env",3))
		print_env(cmd->envp);
}

void	ms_line_str_parsing(t_cmd *cmd)
{
	ms_line_tokenizer(cmd, cmd->line);
	int i = 0;
	while(cmd->line_split[i])
		printf("check:%s\n", cmd->line_split[i++]);
	ms_builtin_func(cmd);

	i = 0;
	while (cmd->line_split[i])
		free(cmd->line_split[i++]);
	free(cmd->line_split);
}

int	main(int ac, char **av, char **envp)
{
	t_cmd	cmd;

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