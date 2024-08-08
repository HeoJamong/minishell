#include "minishell.h"

void	mini_line_parsing(t_cmd *cmd, char *line)
{
	char	*tmp;
	
	if (line == NULL)
		exit (1);
	tmp = ft_strtrim(line, " ");
	if (tmp == NULL)
		exit (1);
	cmd->line_split = ft_split(tmp, ' ');
	if (cmd->line_split == NULL)
		exit (1);
	free(line);
	if (ft_strnstr(cmd->line_split[0], "cd", 2) \
		&& ft_strlen(cmd->line_split[0]) == 2)
		printf("check\n");
	free(tmp);
}

int	main(void)
{
	t_cmd	cmd;

	while (1)
	{
		mini_term_set(&cmd);
		cmd.line = readline("minishell$ ");
		if (cmd.line == NULL)
		{
			printf("exit\n");
			exit (0);
		}
		mini_term_reset(&cmd);
		add_history(cmd.line);
		mini_line_parsing(&cmd, ft_strdup(cmd.line));
		free(cmd.line);
	}
	return (0);
}