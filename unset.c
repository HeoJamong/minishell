#include "minishell.h"

int	ft_envplen(char	**envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return(i);
}

int	ft_contains(char *str, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], str, ft_strlen(str)))
			return (i);
		i++;
	}
	return (-1);
}

int	ft_unset(char *str, t_cmd *cmd)
{
	int		i;
	int		index;
	char	**new;
	int		len;

	len = ft_envplen(cmd->envp);
	i = ft_contains(str, cmd->envp);
	new = malloc(sizeof(char *) * (len));
	if (i == -1)
	{
		printf("없는거임\n");
		return (0);
	}
	else
	{
		index = 0;
		while (index < i)
		{
			new[index] = ft_strdup(cmd->envp[index]);
			free(cmd->envp[index]);
			index++;
		}
		free(cmd->envp[i + 1]);
		while (i + 1 < len)
		{
			new[index] = ft_strdup(cmd->envp[i + 1]);
			free(cmd->envp[i + 1]);
			i++;
		}
		free(cmd->envp);
		cmd->envp = new;
	}
	return (1);
}
