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

int	in_bounds(int x, int y)
{
	if (x < 0 || y < 0 || x >= v.w || y >= v.h)
		return (0);
	return (1);
}

int	v_in_bounds(vec3 pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.x >= v.w || pos.y >= v.h)
		return (0);
	return (1);
}

PFPN	length_squared(vec3 a)
{
	return (a.x * a.x + a.y * a.y + a.z * a.z);
}

PFPN	v_len(vec3 a)
{
	return (sqrtf(a.x * a.x + a.y * a.y + a.z * a.z));
}

/* distance between 2 points */
PFPN	vec_dist(vec3 a, vec3 b)
{
	return (v_len(v_sub(a, b)));
}
