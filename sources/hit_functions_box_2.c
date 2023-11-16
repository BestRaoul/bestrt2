/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static inline void	save_intersection_3(PFPN t[2], PFPN u[2], PFPN v[2],
		const ray *local_r)
{
	vec3	a;
	vec3	k;

	a = local_r->orig;
	k = local_r->dir;
	if (!close_enough(k.y))
	{
		t[4] = (a.y + 1.0) / -k.y;
		t[5] = (a.y - 1.0) / -k.y;
		u[4] = a.x + k.x * t[4];
		v[4] = a.z + k.z * t[4];
		u[5] = a.x + k.x * t[5];
		v[5] = a.z + k.z * t[5];
	}
	else
		(nohit(&t[4], &u[4], &v[4]), nohit(&t[5], &u[5], &v[5]));
}

static inline void	save_intersection_2(PFPN t[2], PFPN u[2], PFPN v[2],
		const ray *local_r)
{
	vec3	a;
	vec3	k;

	a = local_r->orig;
	k = local_r->dir;
	if (!close_enough(k.x))
	{
		t[2] = (a.x + 1.0) / -k.x;
		t[3] = (a.x - 1.0) / -k.x;
		u[2] = a.z + k.z * t[2];
		v[2] = a.y + k.y * t[2];
		u[3] = a.z + k.z * t[3];
		v[3] = a.y + k.y * t[3];
	}
	else
		(nohit(&t[2], &u[2], &v[2]), nohit(&t[3], &u[3], &v[3]));
	save_intersection_3(t, u, v, local_r);
}

// front_back
void	save_box_intersection(PFPN t[2], PFPN u[2], PFPN v[2],
		const ray *local_r)
{
	vec3	a;
	vec3	k;

	a = local_r->orig;
	k = local_r->dir;
	if (!close_enough(k.z))
	{
		t[0] = (a.z - 1.0) / -k.z;
		t[1] = (a.z + 1.0) / -k.z;
		u[0] = a.x + k.x * t[0];
		v[0] = a.y + k.y * t[0];
		u[1] = a.x + k.x * t[1];
		v[1] = a.y + k.y * t[1];
	}
	else
		(nohit(&t[0], &u[0], &v[0]), nohit(&t[1], &u[1], &v[1]));
	save_intersection_2(t, u, v, local_r);
}
