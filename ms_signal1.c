/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signal1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:58:33 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/17 23:05:13 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("> ^C\n");
		exit(130);
	}
}

static void	ms_heredoc_signal(void)
{
	signal(SIGINT, heredoc_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

static void	ms_current_signal(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	ms_term_reset(t_cmd *cmd, int i)
{
	if (i == 0)
		ms_current_signal();
	else if (i == 1)
		ms_heredoc_signal();
	tcsetattr(0, TCSANOW, &cmd->term.current_term);
}
