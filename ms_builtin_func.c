/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 21:32:44 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 15:18:14 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	start_pwd(t_cmd *c)
{
	char	*currdir;

	currdir = getcwd(NULL, 0);
	if (currdir == NULL)
		exit(EXIT_FAILURE);
	printf("%s\n", currdir);
	free(currdir);
	c->sts.process_status = EXIT_SUCCESS;
}

void	start_echo(t_plst *l, t_cmd *c, int ca_cnt)
{
	int	i;

	i = 1;
	if (sn(l->pipe_split[1], "-n", 2) \
		&& sl(l->pipe_split[1]) == 2)
	{
		i = 2;
		while (i + 1 < ca_cnt)
			printf("%s ", l->pipe_split[i++]);
		printf("%s", l->pipe_split[i]);
	}
	else
	{
		if (l->pipe_split[1] == NULL)
			printf("\n");
		else
		{
			while (i + 1 < ca_cnt)
				printf("%s ", l->pipe_split[i++]);
			printf("%s\n", l->pipe_split[i]);
		}
	}
	c->sts.process_status = EXIT_SUCCESS;
}

void	start_export(t_plst *l, t_cmd *c)
{
	if (ft_export(l->pipe_split[1], c) == 0)
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(l->pipe_split[1], STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		c->sts.process_status = EXIT_FAILURE;
	}
	else
		c->sts.process_status = EXIT_SUCCESS;
}

void	start_unset(t_cmd *c, t_plst *l)
{
	if (l->pipe_split[1] != NULL)
		ft_unset(l->pipe_split[1], c);
	c->sts.process_status = EXIT_SUCCESS;
}

int	ms_builtin_func(t_cmd *c, t_plst *l)
{
	if (l->pipe_split[0] == NULL)
		return (0);
	count_ca_cnt(l);
	if (sn(l->pipe_split[0], "cd", 2) && sl(l->pipe_split[0]) == 2)
		start_cd(c, l, l->ca_cnt);
	else if (sn(l->pipe_split[0], "pwd", 3) && sl(l->pipe_split[0]) == 3)
		start_pwd(c);
	else if (sn(l->pipe_split[0], "echo", 4) && sl(l->pipe_split[0]) == 4)
		start_echo(l, c, l->ca_cnt);
	else if (sn(l->pipe_split[0], "exit", 4) && sl(l->pipe_split[0]) == 4)
		start_exit(&l->ca_cnt, l, c);
	else if (sn(l->pipe_split[0], "export", 6) && sl(l->pipe_split[0]) == 6)
	{
		if (l->ca_cnt == 1)
			print_export(c->envp);
		else
			start_export(l, c);
	}
	else if (sn(l->pipe_split[0], "unset", 5) && sl(l->pipe_split[0]))
		start_unset(c, l);
	else if (sn(l->pipe_split[0], "env", 3))
		c->sts.process_status = print_env(c->envp);
	else
		return (1);
	return (0);
}
