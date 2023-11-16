/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

PFPN	mind(PFPN a, PFPN b)
{
	if (a > b)
		return (a);
	return (b);
}

PFPN	maxd(PFPN a, PFPN b)
{
	if (a > b)
		return (a);
	return (b);
}

PFPN	fclamp(PFPN x, PFPN min, PFPN max)
{
	if (x < min)
		return (min);
	else if (x > max)
		return (max);
	return (x);
}
