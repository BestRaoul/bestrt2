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
	while (in(s1[start], set) && s1[start])
		start++;
	i = ft_strlen(s1) - 1;
	while (i > start && in(s1[i], set) && s1[i])
		i--;
	return (ft_substr(s1, start, i - start + 1));
}
