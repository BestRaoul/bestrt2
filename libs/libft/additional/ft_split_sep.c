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

#include "additional.h"

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

static int	get_size(char const *s, char *sep)
{
	int	i;
	int	flag;
	int	count;

	count = 0;
	flag = !in(s[0], sep);
	i = 0;
	while (s[i])
	{
		i++;
		if (flag && (in(s[i], sep) || !s[i]))
		{
			flag = 0;
			count++;
		}
		else if (!in(s[i], sep))
			flag = 1;
	}
	return (count);
}

static char	**gen_res(char **res, char const *s, char *sep)
{
	int	start;
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (i == -1 || s[i])
	{
		if ((i == -1 || in(s[i], sep)) && !in(s[i + 1], sep))
		{
			i++;
			start = i;
			while (s[i] && !in(s[i], sep))
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

char	**ft_split_sep(char const *s, char *sep)
{
	char	**res;
	int		size;

	size = get_size(s, sep);
	res = malloc(sizeof(char *) * size + sizeof(char *));
	if (res == NULL)
		return (NULL);
	if (gen_res(res, s, sep) == NULL)
	{
		free_res(res);
		free(res);
		return (NULL);
	}
	res[size] = NULL;
	return (res);
}