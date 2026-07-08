#include "additional.h"

void	*free_arr(void **arr)
{
	int	i;

	if (arr != NULL)
	{
		i = 0;
		while (arr[i] != NULL)
			free(arr[i++]);
		free(arr);
	}
	return (NULL);
}
