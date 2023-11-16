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

quat	euler_to_quaternion(vec3 euler)
{
	quat	q;
	PFPN	t0;
	PFPN	t1;
	PFPN	t2;
	vec3	t4;

	t0 = cos(euler.y * 0.5f);
	t1 = sin(euler.y * 0.5f);
	t2 = cos(euler.z * 0.5f);
	t4.x = sin(euler.z * 0.5f);
	t4.y = cos(euler.x * 0.5f);
	t4.z = sin(euler.z * 0.5f);
	q.w = t0 * t2 * t4.y + t1 * t4.x * t4.z;
	q.x = t0 * t4.x * t4.y - t1 * t2 * t4.z;
	q.y = t0 * t2 * t4.z + t1 * t4.x * t4.y;
	q.z = t1 * t2 * t4.y - t0 * t4.x * t4.z;
	return (q);
}

vec3	quaternion_to_euler(quat q)
{
	vec3	euler;
	PFPN	sinp;

	euler.x = atan2(2 * (q.w * q.x + q.y * q.z), 1 - 2 * (q.x * q.x + q.y
				* q.y));
	euler.y = asin(2 * (q.w * q.y - q.x * q.z));
	euler.z = atan2(2 * (q.w * q.z + q.x * q.y), 1 - 2 * (q.y * q.y + q.z
				* q.z));
	return (euler);
	sinp = 2 * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1)
		euler.y = copysign(DEG2RAD, sinp);
	else
		euler.y = asin(sinp);
	return (euler);
}

void	quaternion_to_m4x4(quat q, m4x4 m)
{
	m[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
	m[0][1] = 2 * q.x * q.y - 2 * q.z * q.w;
	m[0][2] = 2 * q.x * q.z + 2 * q.y * q.w;
	m[0][3] = 0;
	m[1][0] = 2 * q.x * q.y + 2 * q.z * q.w;
	m[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
	m[1][2] = 2 * q.y * q.z - 2 * q.x * q.w;
	m[1][3] = 0;
	m[2][0] = 2 * q.x * q.z - 2 * q.y * q.w;
	m[2][1] = 2 * q.y * q.z + 2 * q.x * q.w;
	m[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
	m[2][3] = 0;
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

quat	quaternion_multiply(quat a, quat b)
{
	quat	result;

	result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
	result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
	result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
	result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
	return (result);
}

quat	angle_axis(PFPN angle, vec3 axis)
{
	PFPN	s;
	quat	q;

	s = sin(angle / 2);
	q.w = cos(angle / 2);
	q.x = axis.x * s;
	q.y = axis.y * s;
	q.z = axis.z * s;
	return (q);
}
