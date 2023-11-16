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

// Sets the hit record normal vector.
// NOTE: the parameter `outward_normal` is assumed to have unit length.
void	set_face_normal(hit_record *rec, const ray *r,
		const vec3 outward_normal)
{
	rec->front_face = v_dot(r->dir, outward_normal) < 0;
	if (rec->front_face)
		rec->normal = outward_normal;
	else
		rec->normal = v_scal(outward_normal, -1);
}

// If you find a better quicker way to go from local_t to global
// tell me.
PFPN	t2global(const PFPN lt, const ray *local_r, const ray *r,
		const m4x4 fwd)
{
	vec3	temp;

	multiply_matrix_vector(fwd, ray_at(local_r, lt), &temp);
	return (vec_dist(r->orig, temp));
}

Bool	is_interior(const PFPN a, const PFPN b)
{
	if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
		return (False);
	return (True);
}

vec3	get_cube_normal(const int id, hit_record *rec)
{
	return (v3(1 * (id == 3) - 1 * (id == 2), 1 * (id == 5) - 1 * (id == 4), 1
			* (id == 0) - 1 * (id == 1)));
}
