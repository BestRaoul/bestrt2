/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <pkondrac@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gnl.h"

#define DI *(int *)(&data[fd][BUFFER_SIZE + 1])
#define BI *(int *)(&buffer[BUFFER_SIZE + 1])

static void	*free_all(char *line, char *buffer, int r)
{
	if (line != NULL)
		free(line);
	if (buffer != NULL && r == -1)
		BI = BUFFER_SIZE;
	return (NULL);
}

#include <stdio.h>
static int	init_vars(int fd, char *data, char **line)
{
	int	i;

	if (fd < 0 || fd > MAX_OPEN)
		return (0);
	*line = malloc(1);
	if (*line == NULL)
		return (0);
	**line = 0;
	if (*data == -2)
	{
		i = 0;
		while (i < MAX_OPEN)
		{
			*(data + i * (BUFFER_SIZE + 1 + sizeof(int))) = '\0';
			*(int *)(data + BUFFER_SIZE + 1 + i * (BUFFER_SIZE
					+ 1 + sizeof(int))) = BUFFER_SIZE;
			i++;
		}
	}
	return (1);
}
//['H', -42, (int) 0]
static	char	*fill_buffer(int fd, char *buffer, char *line)
{
	int	line_size;
	int	x;
	int	r;

	line_size = 0;
	x = find_nl_buffer(buffer, BI);
//	printf("search from %d -> %d = %d\n", BI, x, buffer[x]);
	while (buffer[x] != '\n')
	{
//		printf("[\'%c\', \'%c\', %d]\n", buffer[BI], buffer[x-1], BI);
		if (extend_and_add(&line, &buffer[BI], line_size, x - BI) == NULL)
			return (free_all(line, buffer, 0));
//		printf("..%s..\n", line);
		line_size += (x - BI);
		r = read(fd, buffer, BUFFER_SIZE);
		if (r == -1)
			return (free_all(line, buffer, -1));
		BI = 0;
		buffer[r] = -42;
		if (r == 0)
		{
			if (line[0] == '\0')
				return (free_all(line, buffer, 0));
			return (line);
		}
		x = find_nl_buffer(buffer, BI);
//		printf("search from %d -> %d = %d\n", BI, x, buffer[x]);
	}
	if (extend_and_add(&line, &buffer[BI], line_size, ++x - BI) == NULL) //?++
		return (free_all(line, buffer, 0));
	BI = x;
	if (line[0] == '\0')
		return (free_all(line, buffer, 0));
	return (line);
}

// [0 ..... | -42 | index]
//     \/
// buffer_size
//
char	*get_next_line(int fd)
{
	static char	data[MAX_OPEN][BUFFER_SIZE + 1 + sizeof(int)] = {{-2}};
	char		*line;
	int			r;

	if (!init_vars(fd, &data[0][0], &line))
		return (NULL);
	if (DI == BUFFER_SIZE)
	{
		r = read(fd, data[fd], BUFFER_SIZE);
		if (r == -1 || r == 0)
			return (free_all(line, &data[fd][0], r));
		DI = 0;
		data[fd][r] = -42;
	}
	return (fill_buffer(fd, &data[fd][0], line));
}
