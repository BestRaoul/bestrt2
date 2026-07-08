
#include "libft.h"

static void	pop_next(t_list *ptr, void (*del)(void*))
{
	if (ptr == NULL)
		return ;
	if (ptr->next != NULL)
		pop_next(ptr->next, del);
	ft_lstdelone(ptr, del);
}

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*tmp;

	if (*lst == NULL)
		return ;
	tmp = (*lst)->next;
	pop_next(tmp, del);
	ft_lstdelone(*lst, del);
	*lst = NULL;
}
