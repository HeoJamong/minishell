/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:41:56 by jheo              #+#    #+#             */
/*   Updated: 2024/12/22 20:27:05 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_num_error_print(t_plst *plst, t_cmd *cmd)
{
	ft_putendl_fd("exit", STDOUT_FILENO);
	ft_putstr_fd("minishell : exit : ", STDERR_FILENO);
	ft_putstr_fd(plst->pipe_split[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	cmd->sts.process_status = 2;
	return (0);
}

void	exit_max_num_check(t_plst *lst_tmp, t_cmd *cmd)
{
	int	i;
	int	exit_num_check;

	i = 0;
	while (lst_tmp->pipe_split[1][i])
	{
		exit_num_check = max_exit_num_check(lst_tmp->pipe_split[1][i], i);
		if (exit_num_check == 1)
			break ;
		i++;
	}
	if (exit_num_check == 0)
		exit(ft_atoll(lst_tmp->pipe_split[1]));
	else
		exit(exit_num_error_print(lst_tmp, cmd));
}

void	eixt_minus_num_check(t_plst *lst_tmp, t_cmd *cmd)
{
	int	i;
	int	exit_num_check;

	i = 0;
	while (lst_tmp->pipe_split[1][i])
	{
		exit_num_check = min_exit_num_check(lst_tmp->pipe_split[1][i], i);
		if (exit_num_check == 1)
			break ;
		i++;
	}
	if (exit_num_check == 0)
		exit(ft_atoll(lst_tmp->pipe_split[1]));
	else
		exit(exit_num_error_print(lst_tmp, cmd));
}

void	set_exit(int flag, t_plst *lst_tmp, t_cmd *cmd)
{
	if (flag == 0 && strlen(lst_tmp->pipe_split[1]) == 19)
		exit_max_num_check(lst_tmp, cmd);
	else if (flag == 1 && strlen(lst_tmp->pipe_split[1]) == 20)
		eixt_minus_num_check(lst_tmp, cmd);
	else if ((flag == 0 && strlen(lst_tmp->pipe_split[1]) < 19 \
	&& strlen(lst_tmp->pipe_split[1]) > 0) || \
	(flag == 1 && strlen(lst_tmp->pipe_split[1]) < 20 && \
	strlen(lst_tmp->pipe_split[1]) > 0))
		exit(ft_atoll(lst_tmp->pipe_split[1]));
	else if (strlen(lst_tmp->pipe_split[1]) > 20)
		exit(exit_num_error_print(lst_tmp, cmd));
}

int	start_exit(int	*ca_cnt, t_plst *lst_tmp, t_cmd *cmd)
{
	int	flag;

	flag = 0;
	if (*(ca_cnt) == 1)
		exit(EXIT_SUCCESS);
	else if (*(ca_cnt) == 2)
	{
		if (exit_num_isdigit(lst_tmp->pipe_split[1], &flag) == 0)
		{
			if (exit_num_error_print(lst_tmp, cmd) == 0)
				exit (2);
		}
		else
			set_exit(flag, lst_tmp, cmd);
	}
	else
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		cmd->sts.process_status = EXIT_FAILURE;
		return (0);
	}
	cmd->sts.process_status = EXIT_SUCCESS;
	return (0);
}
