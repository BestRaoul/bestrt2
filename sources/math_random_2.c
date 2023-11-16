/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

// Returns a random point in the camera defocus disk.
vec3	defocus_disk_sample(void)
{
	vec3	p;

	p = random_in_unit_disk();
	return (v_add(v_add(v.camera_pos, v_scal(v.defocus_disk_u, p.x)),
			v_scal(v.defocus_disk_v, p.y)));
}

vec3	random_v3(void)
{
	return (v3(random_pfpn(), random_pfpn(), random_pfpn()));
}

vec3	random_v3_l(PFPN min, PFPN max)
{
	return (v3(random_pfpn_l(min, max), random_pfpn_l(min, max),
			random_pfpn_l(min, max)));
}

vec3	random_in_unit_sphere(void)
{
	vec3	p;

	while (True)
	{
		p = random_v3_l(-1, 1);
		if (length_squared(p) < 1)
			return (p);
	}
}

vec3	random_unit_vector(void)
{
	return (v_norm(random_in_unit_sphere()));
}

// // In the same hemisphere as the normal
// vec3	random_on_hemisphere(vec3 normal)
// {
// 	vec3	on_unit_sphere;

// 	on_unit_hemisphere = random_unit_vector();
// 	if (v_dot(on_unit_sphere, normal) > 0.0)
// 		return (on_unit_sphere);
// 	else
// 		return (v_scal(on_unit_sphere, -1));
// }