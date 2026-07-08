#include "additional.h"

int	ft_size(const char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

int len(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);	
}
