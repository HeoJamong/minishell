/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_rdr_output1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:58:36 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 15:50:39 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rdr_output_var_init(int *i, t_plst *tmp)
{
	*i = 0;
	tmp->rdr_true = 0;
	tmp->file_fd = 0;
}

int	ms_rdr_output_true(t_cmd *cmd, int i)
{
	t_plst	*tmp;

	tmp = cmd->pipe_lst;
	while (tmp)
	{
		rdr_output_var_init(&i, tmp);
		while (tmp->pipe_split[i])
		{
			if (sn(tmp->pipe_split[i], ">", 1) && sl(tmp->pipe_split[i]) == 1)
			{
				if (rdr_file_output(tmp, &i, 0))
					return (1);
			}
			else if (\
			sn(tmp->pipe_split[i], ">>", 2) && sl(tmp->pipe_split[i]) == 2)
			{
				if (rdr_file_output_append(tmp, &i, 0))
					return (1);
			}
			else
				i++;
		}
		tmp = tmp->next;
	}
	return (0);
}
