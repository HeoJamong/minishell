/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 22:09:49 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/20 15:15:16 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_plst	*ms_lstnew(void)
{
	t_plst	*ptr;

	ptr = (t_plst *)malloc(sizeof(t_plst));
	if (ptr == NULL)
		exit (EXIT_FAILURE);
	ptr->prev = NULL;
	ptr->next = NULL;
	ptr->pipe_split = NULL;
	ptr->heredoc_fd = NULL;
	ptr->ca_cnt = 0;
	return (ptr);
}

t_plst	*ms_lstlast(t_plst *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ms_lstadd_back(t_plst **lst, t_plst *new)
{
	t_plst	*tmp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	tmp = ms_lstlast(*lst);
	new->next = NULL;
	tmp->next = new;
	new->prev = tmp;
}
