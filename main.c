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

void	cmd_path_cat_exec(t_cmd *cmd, t_plst *tmp)
{
	char	**path_split;
	char	*path;
	int		i = 0;

	if (ft_strchr(tmp->pipe_split[0], '/'))
	{
		if (access(tmp->pipe_split[0], X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
			ft_putendl_fd(": No such file or directory", STDERR_FILENO);
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
		}
		path_split = ft_split(path, ':');
		i = 0;
		while (path_split[i])
		{
			path_split[i] = ft_strjoin(path_split[i], "/");
			path_split[i] = ft_strjoin(path_split[i], tmp->pipe_split[0]);
			if (access(path_split[i], X_OK) == 0)
				break ;
			i++;
		}
		if (path_split[i] == NULL)
		{
			ft_putstr_fd(tmp->pipe_split[0], STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
			exit (EXIT_FAILURE);
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
			cmd_path_cat_exec(cmd, tmp);
		waitpid(pid, &exit_sts, 0);
		if (WIFEXITED(exit_sts))
		{
			printf("리턴 값 %d\n", WEXITSTATUS(exit_sts));
			cmd->sts.process_status = exit_sts;
		}
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

void	ms_line_str_parsing(t_cmd *cmd)
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
	if (ms_heredoc_true_input(cmd))
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
	if (ms_rdr_true_output(cmd))
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

int	main(int ac, char **av, char **envp)
{
	t_cmd	cmd;

	(void)av;
	if (ac != 1)
		return (0);
	cmd.envp = set_env(envp);
	cmd.sts.process_status = 0;
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