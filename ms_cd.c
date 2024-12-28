/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 16:44:04 by jheo              #+#    #+#             */
/*   Updated: 2024/12/23 21:27:26 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_no_dir_error(t_plst *lst_tmp, t_cmd *cmd, char *tmp)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(lst_tmp->pipe_split[1], STDERR_FILENO);
	ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	free(tmp);
	cmd->sts.process_status = EXIT_FAILURE;
}

int	cd_slash_error(t_plst *lst_tmp, t_cmd *cmd)
{
	char	*tmp;
	int		dir;

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
	return (1);
}

int	cd_to_ca_cnt_controller(int ca_cnt, t_cmd *cmd)
{
	if (ca_cnt > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		cmd->sts.process_status = EXIT_FAILURE;
		return (0);
	}
	else if (ca_cnt == 1)
		chdir(getenv("HOME"));
	return (1);
}

int	cd_another_error(t_plst *lst_tmp, t_cmd *cmd)
{
	char	*tmp;
	int		dir;

	tmp = ft_strjoin(getcwd(NULL, 0), "/");
	tmp = ft_strjoin(tmp, lst_tmp->pipe_split[1]);
	dir = chdir(tmp);
	if (dir)
	{
		cd_no_dir_error(lst_tmp, cmd, tmp);
		return (0);
	}
	free(tmp);
	return (1);
}

int	start_cd(t_cmd *cmd, t_plst *lst_tmp, int ca_cnt)
{
	if ((ca_cnt > 0 || ca_cnt <= 2) && \
	cd_to_ca_cnt_controller(ca_cnt, cmd) == 0)
		return (0);
	else if (lst_tmp->pipe_split[1] == NULL)
		chdir(getenv("HOME"));
	else if (strncmp("~", lst_tmp->pipe_split[1], 1) == 0)
	{
		if (cd_tild_cntroller(lst_tmp, cmd) == 0)
			return (0);
	}
	else if (strncmp("/", lst_tmp->pipe_split[1], 1) == 0)
	{
		if (cd_slash_error(lst_tmp, cmd) == 0)
			return (0);
	}
	else
	{
		if (cd_another_error(lst_tmp, cmd) == 0)
			return (0);
	}
	cmd->sts.process_status = EXIT_SUCCESS;
	return (0);
}
