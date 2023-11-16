/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:24:00 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/03 13:24:20 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_res(char **res)
{
	int	i;

	i = 0;
	if (!res)
		return (NULL);
	if (!*res)
	{
		return (NULL);
	}
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	return (NULL);
}

static int	get_size(char const *s, char c)
{
	int	i;
	int	flag;
	int	count;

	count = 0;
	flag = s[0] != c;
	i = 0;
	while (s[i])
	{
		i++;
		if (flag && (s[i] == c || !s[i]))
		{
			flag = 0;
			count++;
		}
		else if (s[i] != c)
			flag = 1;
	}
	return (count);
}

static char	**gen_res(char **res, char const *s, char c)
{
	int	start;
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (i == -1 || s[i])
	{
		if ((i == -1 || s[i] == c) && s[i + 1] != c)
		{
			i++;
			start = i;
			while (s[i] && s[i] != c)
				i++;
			if (i - start > 0 && start < (int)ft_strlen(s))
			{
				res[j] = ft_substr(s, start, i - start);
				if (res[j++] == NULL)
					return (NULL);
			}
			i--;
		}
		i++;
	}
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		size;

	size = get_size(s, c);
	res = malloc(sizeof(char *) * size + sizeof(char *));
	if (res == NULL)
		return (NULL);
	if (gen_res(res, s, c) == NULL)
	{
		free_res(res);
		free(res);
		return (NULL);
	}
	res[size] = 0;
	return (res);
}
