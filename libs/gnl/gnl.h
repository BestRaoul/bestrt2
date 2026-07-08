#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# ifndef MAX_OPEN
#  define MAX_OPEN 1024
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
int		find_nl_buffer(const char *buffer, int i);
void	*extend_and_add(char **line, const char *buffer, int line_size, int n);
#endif
