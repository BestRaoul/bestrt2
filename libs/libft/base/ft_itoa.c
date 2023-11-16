/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:38:04 by pkondrac          #+#    #+#             */
/*   Updated: 2022/10/28 12:52:37 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_size(long num)
{
	int	l;

	l = 0;
	if (num == 0)
		return (1);
	if (num < 0)
	{
		num = -num;
		l++;
	}
	while (num > 0)
	{
		num = num / 10;
		l++;
	}
	return (l);
}

char	*ft_itoa(int n)
{
	char	*res;
	long	num;
	int		neg;
	int		l;
	int		i;

	neg = 1;
	if (n < 0)
		neg = -1;
	l = get_size(n);
	res = malloc(sizeof(char) * l + sizeof(char));
	if (!res)
		return (NULL);
	res[0] = '0';
	if (neg == -1)
		res[0] = '-';
	i = l -1;
	num = neg * (long)n;
	while (num > 0)
	{
		res[i--] = (num % 10) + '0';
		num = num / 10;
	}
	res [l] = '\0';
	return (res);
}
