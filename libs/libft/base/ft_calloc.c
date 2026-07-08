
#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*re;

	re = malloc(count * size);
	if (re == NULL)
		return (NULL);
	ft_bzero(re, count * size);
	return (re);
}
