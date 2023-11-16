/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 09:10:25 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/03 13:55:02 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	int	i;
	int	j;

	if ((int)len == -1)
		len = ft_strlen(haystack);
	if (!*needle)
		return ((char *)(haystack));
	i = 0;
	while (haystack[i] && i < (int)len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && i + j < (int)len)
			if (needle[++j] == '\0')
				return ((char *)(&haystack[i]));
		i++;
	}
	return (NULL);
}
