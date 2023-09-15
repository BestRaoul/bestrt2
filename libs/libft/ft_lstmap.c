/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 10:26:37 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/09 11:31:56 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*free_map(void *cnt, t_list **lst, void (*del)(void *))
{
	ft_lstclear(lst, del);
	if (cnt)
		free(cnt);
	return (NULL);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*tmp;
	void	*cnt;

	if (!lst || !f || !del)
		return (NULL);
	cnt = f(lst->content);
	head = ft_lstnew(cnt);
	if (head == NULL)
		free_map(cnt, &head, del);
	while (lst->next)
	{
		lst = lst->next;
		cnt = f(lst->content);
		tmp = ft_lstnew(cnt);
		if (tmp == NULL)
			free_map(cnt, &head, del);
		ft_lstadd_back(&head, tmp);
	}
	return (head);
}
