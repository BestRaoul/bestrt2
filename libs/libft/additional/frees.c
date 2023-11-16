/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 09:20:59 by pkondrac          #+#    #+#             */
/*   Updated: 2022/10/28 15:14:27 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "additional.h"

void	*frees(int count, ...)
{
	va_list	va;
	void	*temp;
	int		i;

	i = 0;
	va_start(va, count);
	while (i < count)
	{
		temp = va_arg(va, void *);
		if (temp != NULL)
			free(temp);
		i++;
	}
	va_end(va);
	return (NULL);
}

//add custom depth freeing
void	*frees2(int count, ...)
{
	va_list	va;
	void	*temp;
	int		i;
	int		mode;

	i = 0;
	va_start(va, count);
	while (i < count)
	{
		mode = va_arg(va, int);
		temp = va_arg(va, void *);
		if (mode == 0)
		{
			if (temp != NULL)
				free(temp);
		}
		else if (mode == 1)
		{
			if (temp != NULL)
				free_arr(temp);
		}
		i++;
	}
	va_end(va);
	return (NULL);
}
