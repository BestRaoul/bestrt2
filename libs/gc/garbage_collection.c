/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 09:20:59 by pkondrac          #+#    #+#             */
/*   Updated: 2022/10/28 15:14:27 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc_malloc.h"

/*Allocate SIZE of bytes, and store in garbage collector*/
void	*gc_malloc(size_t size)
{
	void	*p;
	int		err;

	p = malloc(size);
	if (p == NULL || garbage_collector(ADD, p) == NULL)
	{
		err = errno;
		perror("gc_malloc");
		exit(err);
	}
	return (p);
}
//(const char* fname, int lineno,
//	if (DEBUG_ALLOC)
//	{ printf("m  %s:%d\n", fname, lineno); }

/*Frees PTR, and removes from garbage collector*/
void	gc_free(void *ptr)
{
	garbage_collector(REMOVE, ptr);
	free(ptr);
}
