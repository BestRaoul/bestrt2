#ifndef GC_MALLOC_H
# define GC_MALLOC_H

# include <aio.h>
# include <stdlib.h>
# include <errno.h>
# include <stdio.h>
# include <unistd.h>

# define DEBUG_ALLOC 0

enum e_gc_action {
	ADD,
	REMOVE,
	FREE_ALL,
};

void		*gc_malloc(size_t size);
void		gc_free(void *ptr);
void		*garbage_collector(int action, void *ptr);

#endif