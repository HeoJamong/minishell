#include "minishell.h"

void	cd_hmoe_error(t_plst *lst_tmp, t_cmd *cmd)
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
	}
}

void	cd_dir_error(char *tmp, t_cmd *cmd)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(tmp, STDERR_FILENO);
	ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	free(tmp);
	cmd->sts.process_status = EXIT_FAILURE;
}

int	cd_home_error_controller(t_plst *lst_tmp, t_cmd *cmd)
{
	int		dir;
	char	*tmp;
	if (lst_tmp->pipe_split[1][1] != '/')
	{
		cd_hmoe_error(lst_tmp, cmd);
		return (0);
	}
	else
	{
		tmp = ft_strjoin(ft_strdup(getenv("HOME")), &(lst_tmp->pipe_split[1][1]));
		dir = chdir(tmp);
		if (dir)
		{
			cd_dir_error(tmp, cmd);
			return (0);
		}
		free(tmp);
	}
	return (1);
}

int	start_cd(t_cmd *cmd, t_plst *lst_tmp, int ca_cnt)
{
	char	*tmp;
	int		dir;

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
				if (cd_home_error_controller(lst_tmp, cmd) == 0)
					return (0);
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
