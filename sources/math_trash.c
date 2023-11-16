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

// item ADD/MODIFY
vec3	plane_alligned_add(vec3 base, vec3 add)
{
	return (v3(base.x + add.x * (v.plane != YZ && v.plane != Y && v.plane != Z),
			base.y + add.y * (v.plane != XZ && v.plane != X && v.plane != Z),
			base.z + add.z * (v.plane != XY && v.plane != X && v.plane != Y)));
}

/*Return an array of N + 1 points along a circular path
first and last points are the same*/
vec3	*get_npoints(int n, PFPN r_offset)
{
	vec3	*points;

	points = gc_malloc((n + 1) * sizeof(vec3));
	set_npoints(points, n, r_offset);
	return (points);
}

/*SETS an array of N + 1 points along a circular path
first and last points are the same*/
void	set_npoints(vec3 *points, int n, PFPN r_offset)
{
	PFPN	_a;
	int		i;

	i = 0;
	while (i <= n)
	{
		_a = MYPI * 2 / n * i + r_offset;
		points[i] = v3(sin(_a), cos(_a));
		i++;
	}
}

vec3	look_rotation(vec3 lookfrom, vec3 lookat)
{
	vec3	dir;
	vec3	rot;

	if (v_eq(lookfrom, lookat))
		return (v3(0, MYPI, 0));
	dir = v_sub(lookat, lookfrom);
	dir = v_norm(dir);
	rot = v.camera_rot;
	rot.x = atan2(-dir.y, sqrt(dir.z * dir.z + dir.x * dir.x));
	rot.y = atan2(dir.x, dir.z);
	return (rot);
}

// yaxis.x, xaxis.x);
//(-zaxis.x, sqrt(zaxis.y*zaxis.y + zaxis.z*zaxis.z));
// yaxis = v_cross(xaxis, zaxis);
vec3	look_at(vec3 lookfrom, vec3 lookat, vec3 up)
{
	vec3	xaxis;
	vec3	zaxis;
	PFPN	yaw;
	PFPN	pitch;
	PFPN	roll;

	xaxis = v_norm(v_sub(lookat, lookfrom));
	zaxis = v_norm(v_cross(up, xaxis));
	yaw = atan2(xaxis.x, xaxis.z);
	pitch = atan2(-xaxis.y, sqrt(xaxis.x * xaxis.x + xaxis.z * xaxis.z));
	roll = -atan2(zaxis.y, zaxis.z);
	return (v3(pitch, yaw, roll));
}
//roll is last but can't name him because fuck norm
