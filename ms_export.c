/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:45:12 by jheo              #+#    #+#             */
/*   Updated: 2024/12/20 14:45:57 by jheo             ###   ########.fr       */
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
		printf("%s\n", env[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	check_env(char *str, char *env)
{
	int	i;

	i = 0;
	while (str[i] && env[i] && (str[i] == env[i]) && \
	(str[i] != '=') && (env[i] != '='))
		i++;
	if (i == 0)
		return (0);
	if (str[i] == '\0' && env[i] == '=')
		return(2);
	if (str[i + 1] != '\0')
		return (1);
	if (((str[i] == '=') && (env[i] == '=')))
		return (1);
	return (0);
}

int	find_char_index(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}
