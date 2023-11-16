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

vec3	get_ray_direction(int i, int j)
{
	PFPN	aspect_ratio;
	PFPN	correct_fov;
	vec3	p;

	aspect_ratio = (PFPN)v.w / (PFPN)v.h;
	correct_fov = tan(v.vfov * DEG2RAD * .5);
	p.x = ((2 * ((i + .5) / (PFPN)v.w)) - 1) * aspect_ratio * correct_fov;
	p.y = (1 - (2 * ((j + .5) / (PFPN)v.h))) * correct_fov;
	p.z = 1;
	p = v_norm(p);
	return (p);
}

vec3	ray_at(const ray *r, PFPN t)
{
	return (v_add(r->orig, v_scal(r->dir, t)));
}

vec3	local_p(vec3 p, tfm *transform)
{
	vec3	relative_p;
	vec3	rotated_p;
	vec3	scaled_p;

	relative_p = v_sub(p, transform->pos);
	rotated_p = rotate3(relative_p, transform->rot);
	scaled_p = v3(rotated_p.x / transform->scale.x, rotated_p.y
			/ transform->scale.y, rotated_p.z / transform->scale.z, );
	return (scaled_p);
}

// returns ray IN transformed by T
ray	apply_ray(const ray *in, const m4x4 t)
{
	vec3	p1;
	vec3	p2;

	multiply_matrix_vector(t, in->orig, &p1);
	multiply_matrix_vector(t, v_add(in->orig, in->dir), &p2);
	return ((ray){p1, v_norm(v_sub(p2, p1))});
}
