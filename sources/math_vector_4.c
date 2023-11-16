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

vec3	v_scal(vec3 a, PFPN scalar)
{
	return ((vec3){a.x * scalar, a.y * scalar, a.z * scalar});
}

PFPN	v_dot(vec3 a, vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

vec3	v_cross(vec3 a, vec3 b)
{
	vec3	cp;

	cp.x = a.y * b.z - a.z * b.y;
	cp.y = a.z * b.x - a.x * b.z;
	cp.z = a.x * b.y - a.y * b.x;
	return (cp);
}

vec3	line_intersection(vec3 A, vec3 B, vec3 C, vec3 D)
{
	PFPN	a[2];
	PFPN	b[2];
	PFPN	c[2];
	PFPN	determinant;

	a[0] = B.y - A.y;
	b[0] = A.x - B.x;
	c[0] = a[0] * (A.x) + b[0] * (A.y);
	a[1] = D.y - C.y;
	b[1] = C.x - D.x;
	c[1] = a[1] * (C.x) + b[1] * (C.y);
	determinant = a[0] * b[1] - a[1] * b[0];
	if (determinant == 0)
	{
		return (v3(-1, -1));
	}
	else
	{
		return (v3((b[1] * c[0] - b[0] * c[1]) / determinant,
				(a[0] * c[1] - a[1] * c[0]) / determinant));
	}
}

// sometimes gives wrong Z distance, PROBABLY WHEN one point is behind,
// or smth similar
vec3	project_onto_screen_limits(vec3 p, vec3 dir)
{
	vec3	r;
	vec3	intersections[2];

	intersections[0] = v3(-1, -1);
	intersections[1] = v3(-1, -1);
	if (dir.x > 0)
		intersections[0] = line_intersection(p, v_add(p, dir), v3(v.w - 1, 0),
				v3(v.w - 1, 1));
	else if (dir.x < 0)
		intersections[0] = line_intersection(p, v_add(p, dir), v3(0, 0), v3(0,
					1));
	if (dir.y < 0)
		intersections[1] = line_intersection(p, v_add(p, dir), v3(0), v3(1));
	else if (dir.y > 0)
		intersections[1] = line_intersection(p, v_add(p, dir), v3(0, v.h - 1),
				v3(1, v.h - 1));
	r = intersections[v_in_bounds(intersections[1])];
	if (dir.x != 0)
		r.z = (r.x - p.x) / dir.x;
	else
		r.z = (r.y - p.y) / dir.y;
	if (r.z < 0)
		r.z = 0;
	return (r);
}
