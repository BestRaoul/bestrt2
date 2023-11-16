/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	sign(int n)
{
	if (n >= 0)
		return (1);
	return (-1);
}

PFPN	signd(PFPN n)
{
	if (n < 0)
		return (-1);
	return (1);
}

// 0.0: a
// 1.0: b
vec3	lerp(PFPN t, vec3 a, vec3 b)
{
	return ((vec3){(1. - t) * a.x + t * b.x, (1. - t) * a.y + t * b.y, (1. - t)
		* a.z + t * b.z});
}
