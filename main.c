/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 21:09:00 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 21:26:05 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
