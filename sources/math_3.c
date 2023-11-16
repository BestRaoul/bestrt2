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

vec3	rotate_x(vec3 v, PFPN alpha)
{
	PFPN	cos_a;
	PFPN	sin_a;

	cos_a = cos(alpha);
	sin_a = sin(alpha);
	return (v3(v.x, cos_a * v.y - sin_a * v.z, sin_a * v.y + cos_a * v.z));
}

vec3	rotate_y(vec3 v, PFPN beta)
{
	PFPN	cos_b;
	PFPN	sin_b;

	cos_b = cos(beta);
	sin_b = sin(beta);
	return (v3(cos_b * v.x + sin_b * v.z, v.y, -sin_b * v.x + cos_b * v.z));
}

vec3	rotate_z(vec3 v, PFPN gamma)
{
	PFPN	cos_y;
	PFPN	sin_y;

	cos_y = cos(gamma);
	sin_y = sin(gamma);
	return (v3(cos_y * v.x - sin_y * v.y, sin_y * v.x + cos_y * v.y, v.z));
}

vec3	rotate3(vec3 v, vec3 r)
{
	PFPN	_a;
	PFPN	_b;
	PFPN	_y;
	vec3	_cos;
	vec3	_sin;

	_a = r.x;
	_b = r.y;
	_y = r.z;
	_cos.x = cos(_a);
	_sin.x = sin(_a);
	_cos.y = cos(_b);
	_sin.y = sin(_b);
	_cos.z = cos(_y);
	_sin.z = sin(_y);
	return (v3(v.x * (_cos.y * _cos.z) + v.y * (_sin.x * _sin.y * _cos.z
				- _cos.x * _sin.z) + v.z * (_cos.x * _sin.y * _cos.z + _sin.x
				* _sin.z), v.x * (_cos.y * _sin.z) + v.y * (_sin.x * _sin.y
				* _sin.z + _cos.x * _cos.z) + v.z * (_cos.x * _sin.y * _sin.z
				- _sin.x * _cos.z), v.x * (-_sin.y) + v.y * (_sin.x * _cos.y)
			+ v.z * (_cos.x * _cos.y)));
}

vec3	rotate_around(vec3 v, vec3 p, vec3 axis, PFPN angle)
{
	vec3	rotated;

	v = v_sub(v, p);
	rotated = v;
	rotated.x = v.x * cos(angle) + v_cross(axis, v).x * sin(angle) + axis.x
		* v_dot(axis, v) * (1 - cos(angle));
	rotated.y = v.y * cos(angle) + v_cross(axis, v).y * sin(angle) + axis.y
		* v_dot(axis, v) * (1 - cos(angle));
	rotated.z = v.z * cos(angle) + v_cross(axis, v).z * sin(angle) + axis.z
		* v_dot(axis, v) * (1 - cos(angle));
	v = v_add(rotated, p);
	return (v);
}
