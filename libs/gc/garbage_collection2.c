/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collection2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 09:20:59 by pkondrac          #+#    #+#             */
/*   Updated: 2022/10/28 15:14:27 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc_malloc.h"

typedef struct s_garbage {
	void				*garbage;
	struct s_garbage	*next;
}	t_garbage;

/* static void	print_gt(t_garbage *root)
{
	printf("(%p)", root->garbage);
	root = root->next;
	while (root != NULL)
	{
		printf(" -> (%p)", root->garbage);
		root = root->next;
	}
	printf("\n");
}
*/

static void	*gc_add(t_garbage **current, void *ptr)
{
	t_garbage			*new;

	if (DEBUG_ALLOC)
		write(1, "gc+ \n", 5);
	new = malloc(sizeof(t_garbage));
	if (new == NULL)
		return (NULL);
	new->garbage = ptr;
	new->next = NULL;
	(*current)->next = new;
	*current = new;
	return (new);
}

static void	gc_remove(t_garbage *first, t_garbage **current, void *ptr, int t)
{
	t_garbage	*i;
	t_garbage	*prev;

	(void) t;
	if (DEBUG_ALLOC)
		write(1, "gc- \n", 5);
	i = first->next;
	prev = first;
	while (i != NULL)
	{
		if (i->garbage == ptr)
			break ;
		prev = i;
		i = i->next;
	}
	if (i == NULL)
	{
		dprintf(2, "P: %p\n", ptr);
		write(2, "gc: remove PTR not found.\n", 26);
		exit(127);
	}
	if (*current == i)
		*current = prev;
	prev->next = i->next;
	free(i);
}

static void	free_garbage(t_garbage *g)
{
	if (g == NULL)
		return ;
	if (g->next)
		free_garbage(g->next);
	if (g->garbage)
		free(g->garbage);
	free(g);
}

// t_garbage	*i;
// t_garbage	*j;

static void	gc_liquidate(t_garbage *first, t_garbage **current, int *total)
{
	if (DEBUG_ALLOC)
		write(1, "gc: fa!\n", 8);
	free_garbage(first->next);
	*first = (t_garbage){NULL, NULL};
	(*current) = first;
	*total = 0;
}

/*ADD 		-> adds PTR to garbage stack
REMOVE		-> removes PTR from garbage stack
FREE ALL	-> frees all the garbage*/
void	*garbage_collector(int action, void *ptr)
{
	static t_garbage	first = (t_garbage){NULL, NULL};
	static t_garbage	*current = &first;
	static int			total = 0;

	if (action == ADD)
	{
		if (gc_add(&current, ptr) == NULL)
			return (NULL);
		total++;
	}
	else if (action == REMOVE)
		gc_remove(&first, &current, ptr, total--);
	else if (action == FREE_ALL)
		gc_liquidate(&first, &current, &total);
	else
		(write(2, "GC: incorrect action\n", 21), exit(1));
	return (ptr);
}
