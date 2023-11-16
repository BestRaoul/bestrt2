/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	maybe_flipp(void)
{
	static vec3	prev_pos = (vec3){};
	vec3		pp;
	vec3		p;

	if (v.lookat_toggle)
	{
		pp = v_sub(prev_pos, v.lookat);
		p = v_sub(v.camera_pos, v.lookat);
		if ((signd(pp.x) != signd(p.x) && signd(pp.z) != signd(p.z)))
			v.cam_flipp = !v.cam_flipp;
	}
	prev_pos = v.camera_pos;
}

PFPN	get_move_speed(void)
{
	const PFPN	move_acceleration = 3;
	const PFPN	top_speed = 100;
	static PFPN	s;

	if (v.lookat_toggle)
		s = 0;
	if (!v._right && !v._left && !v._up && !v._down && !v._q && !v._e)
		s = 0;
	s += move_acceleration * v.delta_time;
	s = clamp((interval){0, top_speed}, s);
	if (v._shift)
		return (s / 5);
	return (s);
}

PFPN	get_angular_speed(void)
{
	const PFPN	move_acceleration = MYPI / 3;
	const PFPN	top_speed = MYPI;
	static PFPN	s;

	if (!v.lookat_toggle)
		s = 0;
	if (!v._right && !v._left && !v._up && !v._down && !v._q && !v._e)
		s = 0;
	s += move_acceleration * v.delta_time;
	s = clamp((interval){0, top_speed}, s);
	if (v._shift)
		return (s / 5);
	return (s);
}

void	do_orbit_move(vec3 axis, PFPN angle)
{
	axis = rotate_y(axis, v.camera_rot.y);
	v.camera_pos = rotate_around(v.camera_pos, v.lookat, axis, angle);
}
