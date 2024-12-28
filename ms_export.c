/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:45:12 by jheo              #+#    #+#             */
/*   Updated: 2024/12/26 22:07:22 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_ispossible_export(int c)
{
	if (ft_isalpha(c) || c == '_')
		return (1);
	return (0);
}

int	print_export(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("declare -x ");
		printf("%s\n", env[i]);
		i++;
	}
	return (0);
}

int	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	init_env(char *str, char **env, int e_pointer)
{
	int	i;

	i = 0;
	if (e_pointer != 0)
	{
		while (env[i])
		{
			if (ft_strncmp(env[i], str, e_pointer) == 0)
				return (i);
			i++;
		}
	}
	else
	{
		while (env[i])
		{
			if (ft_strncmp(env[i], str, sl(str)) == 0)
				return (i);
			i++;
		}
	}
	return (0);
}

int	check_env(char *str, char **env, int e_pointer)
{
	int	env_p;

	env_p = init_env(str, env, e_pointer);
	if (e_pointer != 0)
	{
		if (env_p != 0)
			return (env_p);
	}
	else
	{
		if (env_p != 0)
			return (-1);
	}
	return (-2);
}
