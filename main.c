/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 21:09:00 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 15:43:56 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_path_cat_exec(t_cmd *cmd, t_plst *tmp)
{
	void	*tmp_dir;
	char	**path_split;
	char	*path;
	int		i = 0;

	if (ft_strchr(tmp->pipe_split[0], '/'))
	{
		tmp_dir = opendir(tmp->pipe_split[0]);
		if (tmp_dir)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
			ft_putendl_fd(": Is a directory", STDERR_FILENO);
			exit (126);
		}
		free(tmp_dir);
		if (access(tmp->pipe_split[0], X_OK) != 0)
		{
			if (access(tmp->pipe_split[0], F_OK) != 0)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
				ft_putendl_fd(": No such file or directory", STDERR_FILENO);
				exit (127);
			}
			else
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
				ft_putendl_fd(": Permission denied", STDERR_FILENO);
				exit (126);
			}
		}
	}
	else
	{
		while (cmd->envp[i])
		{
			path = ft_envchr(cmd->envp[i++], "PATH");
			if (path)
			{
				path += 5;
				break ;
			}
		}
		if (path == NULL) // 환경변수에 path가 없을 때 처리
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
			ft_putendl_fd(": No such file or directory", STDERR_FILENO);
			exit (127);
		}
		path_split = ft_split(path, ':');
		i = 0;
		while (path_split[i])
		{
			path_split[i] = ft_strjoin(path_split[i], "/");
			path_split[i] = ft_strjoin(path_split[i], tmp->pipe_split[0]);
			if (access(path_split[i], F_OK) == 0)
				break ;
			i++;
		}
		if (path_split[i] == NULL)
		{
			ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
			exit (127);
		}
		free(tmp->pipe_split[0]);
		tmp->pipe_split[0] = ft_strdup(path_split[i]);
		ft_line_split_free(path_split);
	}
	if (execve(tmp->pipe_split[0], tmp->pipe_split, cmd->envp) == -1)
		exit (EXIT_FAILURE);
}

void	cmd_exec(t_cmd *cmd, t_plst *tmp)
{
	int		pid;
	int		exit_sts;
	int		input_fd;
	int		output_fd;

	input_fd = dup(STDIN_FILENO);
	output_fd = dup(STDOUT_FILENO);
	if (tmp->heredoc_true)
	{
		dup2(tmp->heredoc_fd[0], STDIN_FILENO);
		close(tmp->heredoc_fd[0]);
		close(tmp->heredoc_fd[1]);
	}
	if (tmp->rdr_true)
		dup2(tmp->file_fd, STDOUT_FILENO);
	if (ms_builtin_func(cmd, tmp))
	{
		pid = (int)fork();
		if (pid == -1)
			exit (EXIT_FAILURE);
		if (pid == 0)
		{
			ms_term_reset(cmd, 0);
			cmd_path_cat_exec(cmd, tmp);
		}
		ms_term_set(cmd, 1);
		waitpid(pid, &exit_sts, 0);
		if (WIFEXITED(exit_sts))
			cmd->sts.process_status = WEXITSTATUS(exit_sts);
		if (WIFSIGNALED(exit_sts))
		{
			int	sig = WTERMSIG(exit_sts);
			if (sig == SIGINT)
				ft_putstr_fd("^C\n", STDOUT_FILENO);
			else if (sig == SIGQUIT)
				ft_putstr_fd("^\\Quit (core dumped)\n", STDOUT_FILENO);
			cmd->sts.process_status = sig + 128;
		}
		ms_term_set(cmd, 0);
	}
	close(tmp->file_fd);
	dup2(input_fd, STDIN_FILENO);
	dup2(output_fd, STDOUT_FILENO);
}

int	ms_exec(t_cmd *cmd)
{
	if (cmd->sts.pipe_true == 0)
		cmd_exec(cmd, cmd->pipe_lst);
	else
		cmd_pipe_exec(cmd, cmd->pipe_lst);
	return (0);
}

void	ms_line_parsing_exec(t_cmd *cmd)
{
	t_plst	*tmp;
	t_plst	*pipe_tmp;
	int 	i = 0;

	ms_line_tokenizer(cmd, cmd->line);
	if (ms_line_pipe_split(cmd))
	{
		ft_line_split_free(cmd->line_split);
		return ;
	}
	if (ms_rdr_input_true(cmd, 0))
	{
		ft_line_split_free(cmd->line_split);
		tmp = cmd->pipe_lst;
		while (tmp)
		{
			pipe_tmp = tmp;
			tmp = tmp->next;
			free(pipe_tmp->pipe_split);
			free(pipe_tmp->heredoc_fd);
			free(pipe_tmp);
		}
		return ;
	}
	if (ms_rdr_output_true(cmd, 0))
	{
		ft_line_split_free(cmd->line_split);
		tmp = cmd->pipe_lst;
		while (tmp)
		{
			pipe_tmp = tmp;
			tmp = tmp->next;
			free(pipe_tmp->pipe_split);
			free(pipe_tmp->heredoc_fd);
			free(pipe_tmp);
		}
		return ;
	}
	tmp = cmd->pipe_lst;
	while (tmp->next)
	{
		i++;
		tmp = tmp->next;
	}
	cmd->pipe_cnt = i;
	ms_exec(cmd);
	ft_line_split_free(cmd->line_split);
	tmp = cmd->pipe_lst;
	while (tmp)
	{
		pipe_tmp = tmp;
		tmp = tmp->next;
		free(pipe_tmp->pipe_split);
		free(pipe_tmp->heredoc_fd);
		free(pipe_tmp);
	}
}

void	main_var_set(t_cmd *cmd, t_fd *fd, char **envp)
{
	fd->input = dup(STDIN_FILENO);
	fd->output = dup(STDOUT_FILENO);
	cmd->envp = set_env(envp);
	cmd->sts.process_status = EXIT_SUCCESS;
}

void	main_var_reset(t_cmd *cmd, t_fd *fd)
{
	dup2(fd->input, STDIN_FILENO);
	dup2(fd->output, STDOUT_FILENO);
	free(cmd->line);
}

int	main(int ac, char **av, char **envp)
{
	t_cmd	cmd;
	t_fd	fd;

	(void)av;
	if (ac != 1)
		return (0);
	main_var_set(&cmd, &fd, envp);
	while (1)
	{
		ms_term_set(&cmd, 0);
		cmd.line = readline("minishell$ ");
		if (cmd.line == NULL)
		{
			printf("exit\n");
			exit (EXIT_SUCCESS);
		}
		add_history(cmd.line);
		ms_line_parsing_exec(&cmd);
		main_var_reset(&cmd, &fd);
	}
	return (0);
}
