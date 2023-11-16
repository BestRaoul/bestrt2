/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 09:50:31 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/03 11:55:11 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*dest;
	unsigned int	i;

	if (ft_strlen(s) < start + len)
		len = ft_strlen(s) - start;
	if (start >= ft_strlen(s) || (int)len <= 0)
	{
		dest = malloc(sizeof(char));
		if (!dest)
			return (NULL);
		dest[0] = 0;
		return (dest);
	}
	dest = malloc(sizeof(char) * len + sizeof(char));
	if (!dest)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dest[i] = s[i + start];
		i++;
	}
	dest[i] = 0;
	return (dest);
}
