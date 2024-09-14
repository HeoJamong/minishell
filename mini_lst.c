/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:17:06 by jinsecho          #+#    #+#             */
/*   Updated: 2024/09/14 18:23:36 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe_lst	*mini_lstnew(void)
{
	t_pipe_lst	*ptr;

	ptr = (t_pipe_lst *)malloc(sizeof(t_pipe_lst));
	if (ptr == NULL)
		return (NULL);
	ptr->next = NULL;
	return (ptr);
}

t_pipe_lst	*mini_lstlast(t_pipe_lst *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	mini_lstadd_back(t_pipe_lst **pipe_lst, t_pipe_lst *new)
{
	t_pipe_lst	*tmp;

	if (pipe_lst == NULL || new == NULL)
		return ;
	if (*pipe_lst == NULL)
	{	
		*pipe_lst = new;
		return ;
	}
	tmp = mini_lstlast(*pipe_lst);
	new->next = NULL;
	tmp->next = new;
}
