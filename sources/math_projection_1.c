/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_projection_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

// return (rotate3(v_sub(pos, cam_pos), v_scal(cam_rot, -1)));
// multiply_matrix_vector(v.rotation_matrix, v_sub(pos, cam_pos), &out);
vec3	world_to_local(vec3 cam_pos, vec3 pos)
{
	vec3	out;

	return (mult_point_matrix(v_sub(pos, cam_pos), v.rotation_matrix));
	return (out);
}

vec3	reverse_project(vec3 pos, PFPN vfov, PFPN aspect)
{
	PFPN	f;

	f = pos.z * tan(vfov * DEG2RAD * 0.5f);
	pos.x *= f * aspect;
	pos.y *= f;
	return (pos);
}

vec3	project(vec3 pos, PFPN vfov, PFPN aspect)
{
	PFPN	f;

	f = 1.0 / tan(vfov * DEG2RAD * 0.5f);
	if (v.orthographic_toggle)
		f /= 10.0;
	else
		f /= pos.z;
	pos.x *= f / aspect;
	pos.y *= f;
	return (pos);
}

vec3	project_ortho(vec3 pos, PFPN vfov, PFPN aspect)
{
	PFPN	f;

	f = 1.0;
	pos.x *= f / aspect;
	pos.y *= f;
	return (pos);
}

vec3	clip_space_to_viewport(vec3 pos)
{
	pos.x = pos.x * 0.5f + 0.5f;
	pos.y = pos.y * 0.5f + 0.5f;
	return (pos);
}
