/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

Bool	v_eq(vec3 a, vec3 b)
{
	return (a.x == b.x
		 && a.y == b.y
		 && a.z == b.z);
}

vec3	v_norm(vec3 a)
{
	double	len;

	len = v_len(a);
	return (vec3){a.x/len, a.y/len, a.z/len};
}

vec3	v_scal(vec3 a, double scalar)
{
	return (vec3){a.x * scalar, a.y * scalar, a.z * scalar};
}

/* distance between 2 points */
double	vec_dist(vec3 a, vec3 b)
{
	return (fabs(v_len(v_sub(a, b))));
}

double	pt_dist_to_sphere(vec3 pt, vec3 sphere_center, double rad)
{
	vec3	vct;
	vec3	closest_sphere_point;

	vct = v_sub(sphere_center, pt);
	vct = v_norm(vct);
	closest_sphere_point = v_add(sphere_center, v_scal(vct, rad));
	return (vec_dist(pt, closest_sphere_point));
}

vec3	v_cross(vec3 a, vec3 b)
{
	vec3	cp;

	cp.x = a.y * b.z - a.z * b.y;
	cp.y = a.z * b.x - a.x * b.z;
	cp.z = a.x * b.y - a.y * b.x;
	return (cp);
}

double	v_dot(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

double	length_squared(vec3 a)
{
	return a.x*a.x + a.y*a.y + a.z*a.z;
}
