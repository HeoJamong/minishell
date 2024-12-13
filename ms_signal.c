/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:58:33 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/13 22:17:55 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("^C\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	wait_signal_handler(int signum)
{
	if (signum == SIGINT)
		rl_on_new_line();
	else if (signum == SIGQUIT)
		rl_on_new_line();
}

static void	ms_change_signal(void)
{
	signal(SIGINT, change_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

static void	ms_current_signal(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	ms_wait_signal(void)
{
	signal(SIGINT, wait_signal_handler);
	signal(SIGQUIT, wait_signal_handler);
}

void	ms_term_set(t_cmd *cmd, int i)
{
	tcgetattr(0, &cmd->term.current_term);
	tcgetattr(0, &cmd->term.change_term);
	cmd->term.change_term.c_lflag &= ~512;
	if (i == 0)
		ms_change_signal();
	else if (i == 1)
		ms_wait_signal();
	tcsetattr(0, TCSANOW, &cmd->term.change_term);
}

void	ms_term_reset(t_cmd *cmd)
{
	ms_current_signal();
	tcsetattr(0, TCSANOW, &cmd->term.current_term);
}
