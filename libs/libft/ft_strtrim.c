/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:07:16 by pkondrac          #+#    #+#             */
/*   Updated: 2022/10/28 16:37:53 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	in(char const *s, char c)
{
	int	i;

	if (c == '\0')
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	start;
	unsigned int	i;
	char			*res;

	if (!*s1)
	{
		res = malloc(1);
		if (!res)
			return (NULL);
		res[0] = 0;
		return (res);
	}
	start = 0;
	while (in(set, s1[start]) && s1[start])
		start++;
	i = ft_strlen(s1) - 1;
	while (i > start && in(set, s1[i]) && s1[i])
		i--;
	return (ft_substr(s1, start, i - start + 1));
}
