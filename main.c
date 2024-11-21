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

static int	line_pipe_split_find(t_cmd *cmd, t_plst **tmp, int *i, int *k)
{
	t_plst	*pipe_tmp;
	
	if (cmd->line_split[*i + 1] == NULL)
	{
		*tmp = cmd->pipe_lst;
		while (*tmp)
		{	
			pipe_tmp = *tmp;
			*tmp = (*tmp)->next;
			free(pipe_tmp->pipe_split);
			free(pipe_tmp);
		}
		return (1);
	}
	(*tmp)->pipe_split[*k] = NULL;
	*k = 0;
	ms_lstadd_back(&(cmd->pipe_lst), ms_lstnew());
	*tmp = (*tmp)->next;
	(*tmp)->pipe_split = (char **)malloc(sizeof(char *) * cmd->line_i);
	if ((*tmp)->pipe_split == NULL)
		exit (EXIT_FAILURE);
	(*i)++;
	if (ft_strnstr(cmd->line_split[*i], "|", 1) && \
		ft_strlen(cmd->line_split[*i]) == 1)
	{
		*tmp = cmd->pipe_lst;
		while (*tmp)
		{	
			pipe_tmp = *tmp;
			*tmp = (*tmp)->next;
			free(pipe_tmp->pipe_split);
			free(pipe_tmp);
		}
		return (1);
	}
	return (0);
}

void	ms_idx_init(t_tmp *idx)
{
	idx->i = 0;
	idx->k = 0;
}

int	ms_line_pipe_split(t_cmd *cmd)
{
	t_plst	*tmp;
	t_tmp	idx;

	ms_idx_init(&idx);
	if (cmd->line_split[idx.i] == NULL || \
		(ft_strnstr(cmd->line_split[idx.i], "|", 1) && \
		ft_strlen(cmd->line_split[idx.i]) == 1))
		return (1);
	cmd->pipe_lst = ms_lstnew();
	tmp = cmd->pipe_lst;
	tmp->pipe_split = (char **)malloc(sizeof(char *) * (cmd->line_i + 1));
	if (tmp->pipe_split == NULL)
		exit (EXIT_FAILURE);
	while(idx.i < cmd->line_i)
	{
		tmp->pipe_split[(idx.k)++] = cmd->line_split[(idx.i)++];
		if (cmd->line_split[idx.i] == NULL)
		{
			tmp->pipe_split[idx.k] = NULL;
			break ;
		}
		if (ft_strnstr(cmd->line_split[idx.i], "|", 1) && \
			ft_strlen(cmd->line_split[idx.i]) == 1)
		{
			cmd->sts.pipe_true = 1;
			if (line_pipe_split_find(cmd, &tmp, &idx.i, &idx.k))
				return (1);
		}
		tmp->pipe_split[idx.k] = NULL;
	}
	return (0);
}

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

	// if (tmp->heredoc_true);
	if (ms_builtin_func(cmd, tmp))
	{
		pid = (int)fork();
		if (pid == -1)
			exit (EXIT_FAILURE);
		if (pid == 0)
			cmd_path_cat_exec(cmd, tmp);
		waitpid(pid, &exit_sts, 0);
	}
}

int	ms_exec(t_cmd *cmd)
{
	if (cmd->sts.pipe_true == 0)
		cmd_exec(cmd, cmd->pipe_lst);
	else
		cmd_pipe_exec(cmd, cmd->pipe_lst);
	return (0);
}

void	heredoc_input(t_plst *tmp, char *last_word)
{
	char	*str;
	char	*line;
	int		fd[2];
	int		pid = 1;
	int		exit_sts;

	if (pipe(fd) == -1)
		exit (EXIT_FAILURE);
	tmp->heredoc_fd = fd;
	pid = fork();
	if (pid == 0)
	{
		line = ft_strdup("");
		while(1)
		{
			str = readline("> ");
			if (ft_strlen(str) == ft_strlen(last_word) && \
				!ft_strncmp(str, last_word, ft_strlen(last_word)))
				break ;
			line = ft_strjoin(line, str);
			line = ft_strjoin(line, "\n");
			free(str);
		}
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		printf("%s", line);
		exit (EXIT_SUCCESS);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid, &exit_sts, 0);
}

int	ms_handle_heredoc(t_cmd *cmd)
{
	t_plst	*tmp;
	int		i = 0;
	int		tmp_i = 0;
	int		k;

	tmp = cmd->pipe_lst;
	tmp->heredoc_true = 0;
	while (tmp)
	{
		while (tmp->pipe_split[i])
		{
			if (ft_strnstr(tmp->pipe_split[i], "<<", STDERR_FILENO) && \
				ft_strlen(tmp->pipe_split[i]) == STDERR_FILENO)
			{
				if (tmp->pipe_split[i] == NULL)
				{
					ft_putstr_fd("minishell: ", STDERR_FILENO);
					ft_putendl_fd("syntax error near unexpected token `newline'", STDERR_FILENO);
					return (1);
				}
				tmp->heredoc_true = 1;
				heredoc_input(tmp, tmp->pipe_split[i + 1]);
				k = i;
				k += 2;
				if (tmp->pipe_split[k] == NULL)
					tmp->pipe_split[i] = NULL;
				else
				{
					tmp_i = i;
					while (tmp->pipe_split[k])
					{
						tmp->pipe_split[tmp_i] = tmp->pipe_split[k];
						tmp_i++;
						k++;
					}
					tmp->pipe_split[tmp_i] = NULL;
				}
				if (tmp->pipe_split[tmp_i] == NULL)
					break ;
			}
			i++;
		}
		tmp = tmp->next;
	}
	return (0);
}

void	ms_line_str_parsing(t_cmd *cmd)
{
	t_plst	*tmp;
	t_plst	*pipe_tmp;
	int 	i = 0;
	int		k = 0;
	
	ms_line_tokenizer(cmd, cmd->line);
	if (ms_line_pipe_split(cmd))
	{
		ft_line_split_free(cmd->line_split);
		return ;
	}
	if (ms_handle_heredoc(cmd))
		return ;
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
	// 	if (cmd.rdr.fd)
	// 		unlink("tmp.txt");
	}
	return (0);
}