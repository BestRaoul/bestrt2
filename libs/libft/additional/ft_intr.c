/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_intr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 09:20:59 by pkondrac          #+#    #+#             */
/*   Updated: 2022/10/28 15:14:27 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "additional.h"

int	get_intr(int (*compare)(int, int), int *r, int size)
{
	int	i;
	int	best;

	best = r[0];
	i = 0;
	while (i < size)
	{
		if (compare(r[i], best))
			best = r[i];
		i++;
	}
	return (best);
}

//returns index of X in int array, else -1
int	find_intr(int x, int *r, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (x == r[i])
			return (i);
		i++;
	}
	return (-1);
}

int	count_intr(int (*f)(int), int *r, int size)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (i < size)
	{
		count += f(r[i]);
		i++;
	}
	return (count);
}

int	countx_intr(int x, int (*compare)(int, int), int *r, int size)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (i < size)
	{
		count += compare(r[i], x);
		i++;
	}
	return (count);
}
