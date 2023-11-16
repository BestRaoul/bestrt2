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

// if (v._np9)
// NOT_IMPLEMENTED("Flip around Y axis")
static void	rotate_camera_orbit_move(void)
{
	PFPN	deg;
	int		one;

	deg = 15 / (1 + 4 * v._shift);
	one = 1 - 2 * v.cam_flipp;
	if (v.lookat_toggle)
	{
		if (v._np4 || v._np6 || v._np2 || v._np8)
			do_orbit_move(v3(one * v._np8 - one * v._np2,
					one * v._np4 - one * v._np6), deg * DEG2RAD);
	}
	else
	{
		if (v._np4)
			v.camera_rot.y -= one * deg * DEG2RAD;
		if (v._np6)
			v.camera_rot.y += one * deg * DEG2RAD;
		if (v._np2)
			v.camera_rot.x += one * deg * DEG2RAD;
		if (v._np8)
			v.camera_rot.x -= one * deg * DEG2RAD;
	}
}

// orbit or fly rotation movement
void	rotate_camera(void)
{
	PFPN	dist_to;

	dist_to = vec_dist(v.camera_pos, v.lookat);
	if (v._np0)
	{
		v.camera_pos = v3(0, 0, 0);
		v.camera_rot = look_rotation(v3(), v3(0, 0, -1));
		v.lookat_toggle = 0;
	}
	if (v._np1 || v._np3 || v._np7)
	{
		v.camera_pos = v_mult(v_3(dist_to), v3(v._np3, v._np7, v._np1));
		v.camera_pos.x += 1e-8 * v._np7;
		v.lookat_toggle = 1;
	}
	if (v._np5)
	{
		v.orthographic_toggle = !v.orthographic_toggle;
		v._rerender = 1;
	}
	rotate_camera_orbit_move();
}

static void	move_camera_nolook(void)
{
	vec3	move;
	int		one;
	PFPN	move_speed;

	one = 1 - 2 * v.cam_flipp;
	move_speed = get_move_speed();
	move = v3(0, 0, 0);
	if (v._right)
		move = v3(one);
	if (v._left)
		move = v3(-one);
	if (v._up)
		move = v3(0, 0, 1);
	if (v._down)
		move = v3(0, 0, -1);
	if (v._q)
		move = v3(0, one);
	if (v._e)
		move = v3(0, -one);
	move = v_scal(move, move_speed * v.delta_time);
	v.camera_pos = v_add(v.camera_pos, rotate3(move, v.camera_rot));
}

void	move_camera(void)
{
	PFPN	angular_speed;
	int		one;
	vec3	move;

	angular_speed = get_angular_speed();
	one = 1 - 2 * v.cam_flipp;
	if (v.lookat_toggle)
	{
		if (v._q)
			do_orbit_move(v3(one), angular_speed * v.delta_time);
		if (v._e)
			do_orbit_move(v3(-one), angular_speed * v.delta_time);
		if (v._right)
			do_orbit_move(v3(0, -1), angular_speed * v.delta_time);
		if (v._left)
			do_orbit_move(v3(0, 1), angular_speed * v.delta_time);
		if (v._up || v._down)
		{
			move = v3(0, 0, v._up - v._down);
			move = v_scal(move, angular_speed * v.delta_time);
			v.camera_pos = v_add(v.camera_pos, rotate3(move, v.camera_rot));
		}
	}
	else
		move_camera_nolook();
}

void	fov_camera(void)
{
	if (v._scroll)
		v.vfov = 50 + v._scroll;
}
