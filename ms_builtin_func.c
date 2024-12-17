/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 21:32:44 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/17 22:57:47 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	start_pwd(t_cmd *cmd)
{
	char *currdir;

	currdir = getcwd(NULL, 0);
	if (currdir == NULL)
		exit(EXIT_FAILURE);
	printf("%s\n", currdir);
	free(currdir);
	cmd->sts.process_status = EXIT_SUCCESS;
}

void	start_echo(t_plst *lst_tmp, t_cmd *cmd, int ca_cnt)
{
	int i = 1;

	if (ft_strnstr(lst_tmp->pipe_split[1], "-n", 2) && ft_strlen(lst_tmp->pipe_split[1]) == 2)
	{
		i = 2;
		while (i + 1 < ca_cnt)
			printf("%s ", lst_tmp->pipe_split[i++]);
		printf("%s", lst_tmp->pipe_split[i]);
	}
	else
	{
		if (lst_tmp->pipe_split[1] == NULL)
			printf("\n");
		else
		{
			while (i + 1 < ca_cnt)
				printf("%s ", lst_tmp->pipe_split[i++]);
			printf("%s\n", lst_tmp->pipe_split[i]);
		}
	}
	cmd->sts.process_status = EXIT_SUCCESS;
}

void	start_export(t_plst *lst_tmp, t_cmd *cmd)
{
	if (ft_export(lst_tmp->pipe_split[1], cmd) == 0)
	{
		ft_putendl_fd("not a valid identifier", STDERR_FILENO);
		cmd->sts.process_status = EXIT_FAILURE;
	}
	else
		cmd->sts.process_status = EXIT_SUCCESS;
}

int ms_builtin_func(t_cmd *cmd, t_plst *lst_tmp)
{
	char *tmp;
	int dir;
	int ca_cnt = 0;

	if (lst_tmp->pipe_split[0] == NULL)
		return (0);
	while (lst_tmp->pipe_split[ca_cnt])
		ca_cnt++;
	if (ft_strnstr(lst_tmp->pipe_split[0], "cd", 2) && ft_strlen(lst_tmp->pipe_split[0]) == 2)
	{
		if (ca_cnt > 2)
		{
			ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
			cmd->sts.process_status = EXIT_FAILURE;
			return (0);
		}
		else if (ca_cnt == 1)
			chdir(getenv("HOME"));
		else if (strncmp("~", lst_tmp->pipe_split[1], 1) == 0)
		{
			if (ft_strlen(lst_tmp->pipe_split[1]) == 1)
				chdir(getenv("HOME"));
			else
			{
				if (lst_tmp->pipe_split[1][1] != '/')
				{
					tmp = lst_tmp->pipe_split[1];
					dir = chdir(tmp);
					if (dir)
					{
						ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
						ft_putstr_fd(tmp, STDERR_FILENO);
						ft_putendl_fd(": No such file or directory", STDERR_FILENO);
						cmd->sts.process_status = EXIT_FAILURE;
						return (0);
					}
				}
				else
				{
					tmp = ft_strjoin(ft_strdup(getenv("HOME")), &(lst_tmp->pipe_split[1][1]));
					dir = chdir(tmp);
					if (dir)
					{
						ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
						ft_putstr_fd(tmp, STDERR_FILENO);
						ft_putendl_fd(": No such file or directory", STDERR_FILENO);
						free(tmp);
						cmd->sts.process_status = EXIT_FAILURE;
						return (0);
					}
					free(tmp);
				}
			}
		}
		else if (strncmp("/", lst_tmp->pipe_split[1], 1) == 0)
		{
			tmp = lst_tmp->pipe_split[1];
			dir = chdir(tmp);
			if (dir)
			{
				ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
				ft_putstr_fd(tmp, STDERR_FILENO);
				ft_putendl_fd(": No such file or directory", STDERR_FILENO);
				cmd->sts.process_status = EXIT_FAILURE;
				return (0);
			}
		}
		else
		{
			tmp = ft_strjoin(getcwd(NULL, 0), "/");
			tmp = ft_strjoin(tmp, lst_tmp->pipe_split[1]);
			dir = chdir(tmp);
			if (dir)
			{
				ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
				ft_putstr_fd(lst_tmp->pipe_split[1], STDERR_FILENO);
				ft_putendl_fd(": No such file or directory", STDERR_FILENO);
				free(tmp);
				cmd->sts.process_status = EXIT_FAILURE;
				return (0);
			}
			free(tmp);
		}
		cmd->sts.process_status = EXIT_SUCCESS;
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "pwd", 3) && ft_strlen(lst_tmp->pipe_split[0]) == 3)
	{
		start_pwd(cmd);
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "echo", 4) && ft_strlen(lst_tmp->pipe_split[0]) == 4)
	{
		start_echo(lst_tmp, cmd, ca_cnt);
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "exit", 4) && ft_strlen(lst_tmp->pipe_split[0]) == 4)
	{
		start_exit(&ca_cnt, lst_tmp, cmd);
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "export", 6) && ft_strlen(lst_tmp->pipe_split[0]) == 6)
	{
		if (ca_cnt == 1)
			print_export(cmd->envp);
		else
			start_export(lst_tmp, cmd);
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "unset", 5) && ft_strlen(lst_tmp->pipe_split[0]))
	{
		if (lst_tmp->pipe_split[1] != NULL)
			ft_unset(lst_tmp->pipe_split[1], cmd);
		cmd->sts.process_status = EXIT_SUCCESS;
		return (0);
	}
	else if (ft_strnstr(lst_tmp->pipe_split[0], "env", 3))
	{
		print_env(cmd->envp);
		cmd->sts.process_status = EXIT_SUCCESS;
		return (0);
	}
	return (1);
}
