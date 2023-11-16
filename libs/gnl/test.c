#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "get_next_line.h"
int	main(int ac, char **av)
{
	if (ac <= 1) return (0);
	char *name = av[1];

	int fd = open(name, O_RDONLY);
	char *nline = NULL;
	
	for (int i=0; i<5; i++)
	{
		printf("LINE %d\n", i+1);
		nline = get_next_line(fd);
		printf("%s", nline);
		free(nline);
	}
	close(fd);
}
