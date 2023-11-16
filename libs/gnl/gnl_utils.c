/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <pkondrac@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gnl.h"

int	find_nl_buffer(const char *buffer, int i)
{
	while (i < BUFFER_SIZE)
	{
		if (buffer[i] == '\n' || buffer[i] == -42)
			return (i);
		i++;
	}
	return (i);
}

static void	*ft_memcpy(char *restrict dst, const char *restrict src, int n)
{
	int	i;

	i = 0;
	if (dst == src)
		return (dst);
	while (i < n)
	{
		*(dst + i) = *(src + i);
		i++;
	}
	return (dst);
}

void	*extend_and_add(char **line, const char *buffer, int line_size, int n)
{
	char	*tmp;

	tmp = malloc((line_size + n) * sizeof(char) + sizeof(char));
	if (tmp == NULL)
		return (NULL);
	ft_memcpy(tmp, *line, line_size);
	ft_memcpy(&tmp[line_size], buffer, n);
	tmp[line_size + n] = 0;
	free(*line);
	*line = tmp;
	return (*line);
}

/*
*line = realloc(*line, line_size + n + 1);
if (*line == NULL)
	return (NULL);
ft_memcpy(&((*line)[line_size]), buffer, n);
(*line)[line_size + n] = 0;
*/
