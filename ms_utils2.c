/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 22:06:40 by jheo              #+#    #+#             */
/*   Updated: 2024/12/26 22:07:29 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_cmd_check(t_plst *l)
{
	int	i;

	i = 1;
	while (l->pipe_split[1][i])
	{
		if (l->pipe_split[1][i] != 'n')
			return (0);
		i++;
	}
	return (1);
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
