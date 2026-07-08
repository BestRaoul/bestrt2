#include "additional.h"

int	in(const char c, const char *str)
{
	if (ft_find(c, str) != -1)
		return (1);
	return (0);
}
