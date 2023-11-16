/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upate_item.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

vec3	project_onto_line(vec3 A, vec3 B, vec3 C)
{
	vec3	_ab;
	vec3	_ac;
	vec3	_p;
	PFPN	dot_ab;
	PFPN	dot_ac;

	_ab.x = B.x - A.x;
	_ab.y = B.y - A.y;
	_ac.x = C.x - A.x;
	_ac.y = C.y - A.y;
	dot_ab = _ab.x * _ab.x + _ab.y * _ab.y;
	dot_ac = _ac.x * _ab.x + _ac.y * _ab.y;
	_p.x = (dot_ac / dot_ab) * _ab.x;
	_p.y = (dot_ac / dot_ab) * _ab.y;
	return (v_add(_p, A));
}

PFPN	tddd(vec3 from, vec3 to, vec3 dir)
{
	vec3	v;
	PFPN	d;

	v = from_to(from, to);
	d = sqrt(v.x * v.x + v.y * v.y);
	if (v_dot(v, dir) < 0)
		return (-d);
	return (d);
}

// in Radians
//[0, MYPI]
PFPN	get_angle(vec3 dir)
{
	PFPN	a;

	a = atan2(dir.y, dir.x);
	if (a < 0)
		return (a + MYPI);
	return (a);
}

void	v_round(vec3 *in, PFPN precision)
{
	in->x = (int)(in->x / precision) * precision;
	in->y = (int)(in->y / precision) * precision;
	in->z = (int)(in->z / precision) * precision;
}

// PFPN directioned_distance(vec3 from, vec3 to, vec3 dir)
// {
// 	PFPN d = vec_dist(from, to);

// 	vec3 my_dir = from_to(from, to);
// 	if (signd(my_dir.x) != signd(dir.x) && signd(my_dir.y) != signd(dir.y))
// 		return (-d);
// 	return (d);
// }
// PFPN distanceToLine(vec3 A, vec3 B, vec3 C) {
//     PFPN numerator = fabs((B.x - A.x) * (A.y - C.y) - (A.x - C.x) * (B.y
// - A.y));
//     PFPN denominator = sqrt((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y
// - A.y));
//     PFPN distance = numerator / denominator;
//     return (distance);
// }
