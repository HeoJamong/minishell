#include "minishell.h"

void	alter_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		ft_putstr_fd("^C\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	ms_alter_signal(void)
{
	signal(SIGINT, alter_signal_handler);
	signal(SIGQUIT, alter_signal_handler);
}

static void	ms_current_signal(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	ms_term_set(t_cmd *cmd)
{
	tcgetattr(0, &cmd->term.current_term);
	tcgetattr(0, &cmd->term.alter_term);
	cmd->term.alter_term.c_lflag &= ~512;
	ms_alter_signal();
	tcsetattr(0, TCSANOW, &cmd->term.alter_term);
}

void	ms_term_reset(t_cmd *cmd)
{
	ms_current_signal();
	tcsetattr(0, TCSANOW, &cmd->term.current_term);
}