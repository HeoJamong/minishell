/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_replace_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 19:34:11 by jinsecho          #+#    #+#             */
/*   Updated: 2024/09/13 19:34:26 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	replace_env_find(t_cmd *cmd, char **ptr, char *line, int *n)
{
	char	*tmp;
	int		i;

	i = 0;
	while (ft_isalnum(line[*n]) || line[*n] == '_')
		(*n)++;
	tmp = (char *)malloc(sizeof(char) * (*n));
	if (tmp == NULL)
		exit (EXIT_FAILURE);
	ft_strlcpy(tmp, line + 1, *n);
	while (cmd->envp[i])
	{
		if (ft_envchr(cmd->envp[i], tmp))
		{
			*ptr = ft_strdup(ft_strchr(cmd->envp[i], '=') + 1);
			free(tmp);
			return ;
		}
		i++;
	}
	*ptr = ft_strdup("");
	free(tmp);
}

static char	*ms_str_replace_env(t_cmd *cmd, char *line, int *n)
{
	char	*ptr;

	*n = 1;
	if (line[*n] == '?')
	{
		*n = 2;
		ptr = ft_itoa(0);
	}
	else if (ft_isalpha(line[*n]) || line[*n] == '_')
		replace_env_find(cmd, &ptr, line, n);
	else if (ft_isdigit(line[*n]))
	{
		*n = 2;
		ptr = ft_strdup("");
	}
	else
		ptr = ft_strdup("");
	return (ptr);
}

void	ms_line_replace_env(t_cmd *cmd, char **ptr, char *line)
{
	t_env_var	env;

	env.cnt = 0;
	while ((*ptr)[env.cnt] != '$' && (*ptr)[env.cnt])
		(env.cnt)++;
	while ((*ptr)[env.cnt])
	{
		env.env_tmp = ms_str_replace_env(cmd, &(*ptr)[env.cnt], &env.n);
		if (env.n == 1)
			(env.cnt)++;
		else
		{
			env.len = ft_strlen(line) + ft_strlen(env.env_tmp);
			*ptr = ft_realloc(*ptr, env.len + 1);
			env.str_tmp = ft_strdup(&(*ptr)[env.cnt] + env.n);
			ft_strlcpy(&(*ptr)[env.cnt], env.env_tmp, ft_strlen(env.env_tmp) + 1);
			ft_strlcat(*ptr, env.str_tmp, env.len + 1);
			(env.cnt) += ft_strlen(env.env_tmp);
			free(env.str_tmp);
		}
		while ((*ptr)[env.cnt] != '$' && (*ptr)[env.cnt])
			(env.cnt)++;
		free(env.env_tmp);
	}
}