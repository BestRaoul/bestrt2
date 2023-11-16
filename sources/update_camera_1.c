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

static void	set_camera_vup(void);
static void	set_raytrace_values(void);
static void	set_rotation_matrix(void);

void	update_camera(void)
{
	static vec3		prev_pos = (vec3){0, 0, 0};
	static vec3		prev_rot = (vec3){0, 0, 0};
	static PFPN		prev_fov = 0;
	vec3			temp_lookat;
	int				change;

	temp_lookat = v.lookat;
	if (v.selected != NULL)
		v.lookat = v.selected->pos;
	(fov_camera(), move_camera(), rotate_camera());
	change = (!v_eq(prev_pos, v.camera_pos) || !v_eq(prev_rot, v.camera_rot));
	change |= (prev_fov != v.vfov || v.camera_change);
	v.camera_change = 0;
	if (change || v._p)
	{
		v._rerender = 1;
		set_camera_vup();
		if (v.lookat_toggle && !v_eq(v.camera_pos, v.lookat))
			v.camera_rot = look_rotation(v.camera_pos, v.lookat);
		(set_rotation_matrix(), set_raytrace_values());
	}
	prev_pos = v.camera_pos;
	prev_rot = v.camera_rot;
	prev_fov = v.vfov;
	v.lookat = temp_lookat;
}

// print_pos(pp, "pp: ");
// print_pos(p, "p: ");
// printf("%f != %f and %f != %f\n", signd(pp.x), signd(p.x),
// 	signd(pp.z), signd(p.z));
static void	set_camera_vup(void)
{
	vec3		temp_lookat;
	vec3		forward;
	vec3		right;
	vec3		new_up;

	maybe_flipp();
	temp_lookat = v.lookat;
	if (!v.lookat_toggle)
		temp_lookat = v_add(v.camera_pos, rotate3(v3(0, 0, 1), v.camera_rot));
	forward = v_norm(v_sub(temp_lookat, v.camera_pos));
	right = v_norm(v_cross(forward, v3(0, 1, 0)));
	new_up = v_scal(v_cross(forward, right), -1);
	if (v.cam_flipp)
		new_up = v_scal(new_up, -1);
	v.vup = new_up;
	v.camera_dir = rotate3(v3(0, 0, 1), v.camera_rot);
}

// quat q3 = angle_axis(v.camera_rot.z, v3(0,0,1));
// q = quaternion_multiply(q3, q);
static void	set_rotation_matrix(void)
{
	quat	q;
	quat	q2;

	q = angle_axis(v.camera_rot.x, v3(1, 0, 0));
	q2 = angle_axis(v.camera_rot.y, v3(0, 1, 0));
	q = quaternion_multiply(q2, q);
	quaternion_to_m4x4(q, v.rotation_matrix);
}

typedef struct s_rv_data {
	vec3	_u;
	vec3	_v;
	vec3	_w;
	vec3	viewport_u;
	vec3	viewport_v;
}				t_rv_data;

static void	set_pixel_delta(vec3 temp_lookat, t_rv_data *d)
{
	PFPN	viewport_height;
	PFPN	viewport_width;

	viewport_height = 2 * tan(v.vfov * DEG2RAD / 2) * v.focus_dist;
	viewport_width = viewport_height * (PFPN)v.w / (PFPN)v.h;
	d->_w = v_norm(v_sub(v.camera_pos, temp_lookat));
	d->_u = v_norm(v_cross(v.vup, d->_w));
	d->_v = v_cross(d->_w, d->_u);
	d->viewport_u = v_scal(d->_u, viewport_width);
	d->viewport_v = v_scal(d->_v, -viewport_height);
	v.pixel_delta_u = v_scal(d->viewport_u, 1.0 / v.w);
	v.pixel_delta_v = v_scal(d->viewport_v, 1.0 / v.h);
}

static void	set_raytrace_values(void)
{
	t_rv_data	d;
	vec3		temp_lookat;
	vec3		viewport_upper_left;
	PFPN		defocus_radius;

	if (v.lookat_toggle)
		temp_lookat = v.lookat;
	else
		temp_lookat = v_add(v.camera_pos, rotate3(v3(0, 0, 1), v.camera_rot));
	v.camera_center = v.camera_pos;
	set_pixel_delta(temp_lookat, &d);
	viewport_upper_left = v_sub(v_sub(v_sub(v.camera_center, v_scal(d._w,
						v.focus_dist)), v_scal(d.viewport_u, .5)),
			v_scal(d.viewport_v, .5));
	v.pixel00_loc = v_add(viewport_upper_left, v_scal(v_add(v.pixel_delta_u,
					v.pixel_delta_v), .5));
	defocus_radius = v.focus_dist * tan(v.defocus_angle * DEG2RAD / 2);
	v.defocus_disk_u = v_scal(d._u, defocus_radius);
	v.defocus_disk_v = v_scal(d._v, defocus_radius);
}
