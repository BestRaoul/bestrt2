/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comarision.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 09:20:59 by pkondrac          #+#    #+#             */
/*   Updated: 2022/10/28 15:14:27 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "additional.h"

int	eq(int x, int y)
{
	return (x == y);
}

int	less(int x, int y)
{
	return (x < y);
}

int	less_eq(int x, int y)
{
	return (x <= y);
}

int	more(int x, int y)
{
	return (x > y);
}

int	more_eq(int x, int y)
{
	return (x >= y);
}
