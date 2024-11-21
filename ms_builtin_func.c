/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 21:32:44 by jinsecho          #+#    #+#             */
/*   Updated: 2024/11/21 02:00:10 by jinsecho         ###   ########.fr       */
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

int	ms_builtin_func(t_cmd *cmd, t_plst *lst_tmp)
{
	char	*tmp;
	// char	buf[1000];
	int		dir;
	int		ca_cnt = 0;

	if (lst_tmp->pipe_split[0] == NULL)
		return (0);
	while (lst_tmp->pipe_split[ca_cnt])
		ca_cnt++;
	if (ft_strnstr(lst_tmp->pipe_split[0], "cd", 2) && ft_strlen(lst_tmp->pipe_split[0]) == 2)
	{
		if (ca_cnt > 2)
			printf("minishell: cd: too many arguments\n");
		else if (ca_cnt == 1)
			chdir(getenv("HOME"));
		else if (strncmp("~", lst_tmp->pipe_split[1], 1) == 0)
		{
			if (ft_strlen(lst_tmp->pipe_split[1]) == 1)
				chdir(getenv("HOME"));
			else if (lst_tmp->pipe_split[1][1] != '/')
			{
				tmp = lst_tmp->pipe_split[1];
				dir = chdir(tmp);
				if (dir)
					printf("minishell: cd: %s: No such file or directory\n", tmp);
			}
			else
			{
				tmp = ft_strjoin(ft_strdup(getenv("HOME")), &(lst_tmp->pipe_split[1][1]));
				dir = chdir(tmp);
				if (dir)
					printf("minishell: cd: %s: No such file or directory\n", tmp);
				free(tmp);
			}
		}
		else if (strncmp("/", lst_tmp->pipe_split[1], 1) == 0)
		{
			tmp = lst_tmp->pipe_split[1];
			dir = chdir(tmp);
			if (dir)
				printf("minishell: cd: %s: No such file or directory\n", tmp);
		}
		else
		{
			tmp = ft_strjoin(getcwd(NULL, 0), "/");
			tmp = ft_strjoin(tmp, lst_tmp->pipe_split[1]);
			dir = chdir(tmp);
			if (dir)
				printf("minishell: cd: %s: No such file or directory\n", lst_tmp->pipe_split[1]);
			free(tmp);
		}
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "pwd", 3) && ft_strlen(lst_tmp->pipe_split[0]) == 3)
	{
		char	*currdir;

		currdir = getcwd(NULL, 0);
		if (currdir == NULL)
			exit (EXIT_FAILURE);
		printf("%s\n", currdir);
		free(currdir);
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "echo", 4) && ft_strlen(lst_tmp->pipe_split[0]) == 4)
	{
		int	i = 1;

		if (ft_strnstr(lst_tmp->pipe_split[1], "-n", 2) && ft_strlen(lst_tmp->pipe_split[1]) == 2)
		{
			i = 2;
			while (i + 1 < ca_cnt)
				printf("%s ", lst_tmp->pipe_split[i++]);
			printf("%s", lst_tmp->pipe_split[i]);
		}
		else
		{
			while (i + 1 < ca_cnt)
				printf("%s ", lst_tmp->pipe_split[i++]);
			printf("%s\n", lst_tmp->pipe_split[i]);
		}
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "exit", 4) && ft_strlen(lst_tmp->pipe_split[0]) == 4)
	{
		printf("exit\n");
		int i = 0;
		int	flag;
		int	exit_num_check;

		flag = 0;
		if (ca_cnt == 1)
			exit(EXIT_SUCCESS);
		else if (ca_cnt == 2)
		{
			printf("호출 됐나요?\n");
			if (exit_num_isdigit(lst_tmp->pipe_split[1], &flag) == 0)
				printf("숫자 아님\n");
			else
			{
				if (flag == 0 && strlen(lst_tmp->pipe_split[1]) == 19)
				{
					while (lst_tmp->pipe_split[1][i])
					{
						exit_num_check = max_exit_num_check(lst_tmp->pipe_split[1][i], i);
						if (exit_num_check == 1)
							break;
						i++;
					}
					if (exit_num_check == 0)
						printf("exit: %lld\n", ft_atol(lst_tmp->pipe_split[1]));
					else
						printf("올바른 값이 아님\n");
				}
				else if (flag == 1 && strlen(lst_tmp->pipe_split[1]) == 20)
				{
					while (lst_tmp->pipe_split[1][i])
					{
						exit_num_check = min_exit_num_check(lst_tmp->pipe_split[1][i], i);
						if (exit_num_check == 1)
							break;
						i++;
					}
					if (exit_num_check == 0)
						printf("exit: %lld\n", ft_atol(lst_tmp->pipe_split[1]));
					else
						printf("올바른 값이 아님\n");
				}
				else if ((flag == 0 && strlen(lst_tmp->pipe_split[1]) < 19 && strlen(lst_tmp->pipe_split[1]) > 0) \
				|| (flag == 1 && strlen(lst_tmp->pipe_split[1]) < 20 && strlen(lst_tmp->pipe_split[1]) > 0))
					printf("exit: %lld\n",ft_atol(lst_tmp->pipe_split[1]));
			}
		}
		else
			printf("minishell: exit: too many arguments\n");
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "export", 6) && ft_strlen(lst_tmp->pipe_split[0]) == 6)
	{
		if (ft_export(lst_tmp->pipe_split[1], cmd) == 1)
			printf("ok\n");
		print_env(cmd->envp);
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "unset", 5) && ft_strlen(lst_tmp->pipe_split[0]))
	{
		if (ft_unset(lst_tmp->pipe_split[1], cmd) == 1)
			printf("ok\n");
		print_env(cmd->envp);
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "env",3))
	{
		print_env(cmd->envp);
		return (0);
	}
	// else if (ft_strnstr(lst_tmp->pipe_split[0], "<<", 2))
	// {
	// 	if (here_doc(lst_tmp->pipe_split[1], cmd))
	// 		if (here_doc_pipe(cmd->rdr.line, cmd))
	// 		{
	// 			printf("%ld\n", read(cmd->rdr.fd, buf, 100));
	// 			printf("%s\n",buf);
	// 		}
	// 	return (0);
	// }
	// else if (ft_strnstr(lst_tmp->pipe_split[0], "<", 1))
	// {
	// 	if (input_redirect(lst_tmp->pipe_split[1]))
	// 		printf("gooood\n");
	// 	return (0);
	// // }
	// else if (ft_strnstr(lst_tmp->pipe_split[0], ">", 1))
	// {
	// 	if (output_redirect(lst_tmp->pipe_split[1]))
	// 		printf("good\n");
	// 	return (0);
	// }
	return (1);
}