/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 19:29:01 by jinsecho          #+#    #+#             */
/*   Updated: 2024/03/12 18:59:22 by jinsecho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{	
		*lst = new;
		return ;
	}
	tmp = ft_lstlast(*lst);
	new->next = NULL;
	tmp->next = new;
}
/*
int		main()
{
	t_list	**ptr;
	t_list	ptr2;
	t_list	new;

	ptr2.content = "jakim";
	new.content = "jinse";
	ptr = (t_list **)malloc(sizeof(t_list *));
	if (ptr == NULL)
		return (0);
	new.next = NULL;
	ptr2.next = NULL;
	*ptr = &ptr2;
	ft_lstadd_back(ptr, &new);
	while(*ptr)
	{
		printf("%s", (char *)(*ptr) -> content);
		*ptr = (*ptr) -> next;
	}
	return (0);
}
*/
