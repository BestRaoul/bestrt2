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

static void	fov_camera();
static void	rotate_camera();
static void	move_camera();

static void set_camera_vup();
static void set_raytrace_values();
static void set_rotation_matrix();

void    update_camera(void)
{
	static vec3 prev_pos = (vec3){0,0,0};
	static vec3 prev_rot = (vec3){0,0,0};
	static double	prev_fov = 0;

	fov_camera();
	move_camera();
	rotate_camera();

	int change = 0;
	change |= !v_eq(prev_pos, v.camera_pos);
	change |= !v_eq(prev_rot, v.camera_rot);
	change |= prev_fov != v.vfov;
	change |= v.camera_change; v.camera_change = 0;
    
	if (change || v._p)
	{
		v._rerender = 1;
		set_camera_vup();

		if (v.lookat_toggle && !v_eq(v.camera_pos, v.lookat))
		{
			v.camera_rot = lookRotation(v.camera_pos, v.lookat);
		}

		set_rotation_matrix();
		set_raytrace_values();
	}

	//draw_projected_line(v.lookat, v_add(v.lookat, v.vup), v3(1,1,0));
	//draw_projected_line(v.lookat, v_add(v.lookat, v3(0, 1, 0)), v3(0,1,1));

	prev_pos = v.camera_pos;
	prev_rot = v.camera_rot;
	prev_fov = v.vfov;
}

quat	angleAxis(double angle, vec3 axis) {
    double s = sin(angle / 2);
    quat q;
    q.w = cos(angle / 2);
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
    return q;
}

void	quaternion_to_m4x4(quat q, m4x4 m) {
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

quat	quaternion_multiply(quat a, quat b) {
    quat result;
    result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    return result;
}

static void set_camera_vup()
{
	vec3 temp_lookat = v.lookat_toggle ? v.lookat : v_add(v.camera_pos, rotate3(v3(0,0,1), v.camera_rot));

	vec3 forward = v_norm(v_sub(temp_lookat, v.camera_pos));
	vec3 right = v_norm(v_cross(forward, v3(0, 1, 0)));
	vec3 new_up = v_scal(v_cross(forward, right), -1);

	v.vup = new_up;

	if (v.cam_flipp)
	{
		v.vup = v_scal(v.vup, -1);
	}

	static vec3 prev_pos = (vec3){};
	if (v.lookat_toggle)
	{
		vec3 pp = v_sub(prev_pos, v.lookat);
		vec3 p = v_sub(v.camera_pos, v.lookat);
		if ((signd(pp.x) != signd(p.x) && signd(pp.z) != signd(p.z)))
		{
			print_pos(pp, "pp: ");
			print_pos(p, "p: ");
			printf("%f != %f and %f != %f\n", signd(pp.x), signd(p.x), signd(pp.z), signd(p.z));
			v.cam_flipp = !v.cam_flipp;
		}
	}
	prev_pos = v.camera_pos;
}

static void	set_rotation_matrix()
{
    quat q = angleAxis(v.camera_rot.x,v3(1,0,0));
    quat q2 = angleAxis(v.camera_rot.y, v3(0,1,0));
	// quat q3 = angleAxis(v.camera_rot.z, v3(0,0,1));
    q = quaternion_multiply(q2, q);
	//q = quaternion_multiply(q3, q);
	quaternion_to_m4x4(q, v.rotation_matrix);
}

static void set_raytrace_values()
{
	double	aspect_ratio = (double)v.w/(double)v.h;

	v.lookfrom = v.camera_pos;
	vec3 temp_lookat = v.lookat_toggle ? v.lookat : v_add(v.camera_pos, rotate3(v3(0,0,1), v.camera_rot));

	//...

	vec3	_u, _v, _w;

	//initialize
	v.camera_center = v.lookfrom;
	double theta = v.vfov * DEG2RAD;
	double h = tan(theta/2);
	double viewport_height = 2 * h * v.focus_dist;
	double viewport_width = viewport_height * aspect_ratio;

	//Calculate the u,v,w unit basis vectors for the camera coordinate frame.
	_w = v_norm(v_sub(v.lookfrom, temp_lookat));
	_u = v_norm(v_cross(v.vup, _w));
	_v = v_cross(_w, _u);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = v_scal(_u, viewport_width);    // Vector across viewport horizontal edge
	vec3 viewport_v = v_scal(_v, -viewport_height);  // Vector down viewport vertical edge

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	v.pixel_delta_u = v_scal(viewport_u, 1.0/v.w);
	v.pixel_delta_v = v_scal(viewport_v, 1.0/v.h);

	// Calculate the location of the upper left pixel.
	vec3 viewport_upper_left = v_sub(v_sub(v_sub(
		v.camera_center,
		v_scal(_w, v.focus_dist)),
		v_scal(viewport_u, .5)),
		v_scal(viewport_v, .5));
	v.pixel00_loc = v_add(viewport_upper_left,
		v_scal(v_add(v.pixel_delta_u, v.pixel_delta_v), .5) );

	 // Calculate the camera defocus disk basis vectors.
	double defocus_radius = v.focus_dist * tan(v.defocus_angle*DEG2RAD / 2);
	v.defocus_disk_u = v_scal(_u, defocus_radius);
	v.defocus_disk_v = v_scal(_v, defocus_radius);
}

// -----

double	get_move_speed()
{
	const double	move_acceleration = 3;
	const double	top_speed = 100;
	static double	s;

	if (v.lookat_toggle)
		s = 0;
	if (!v._right && !v._left && !v._up && !v._down && !v._q && !v._e)
		s = 0;

	s += move_acceleration*v.delta_time;
	s = clamp((interval){0, top_speed}, s);

	if (v._shift)
		return s/5;

	return s;
}

double	get_angular_speed()
{
	const double	move_acceleration = MYPI/3;
	const double	top_speed = MYPI;
	static double	s;

	if (!v.lookat_toggle)
		s = 0;
	if (!v._right && !v._left && !v._up && !v._down && !v._q && !v._e)
		s = 0;

	s += move_acceleration*v.delta_time;
	s = clamp((interval){0, top_speed}, s);

	if (v._shift)
		return s/5;

	return s;
}

void	do_orbit_move(vec3 axis, double angle)
{
	axis = rotate_y(axis, v.camera_rot.y);
	v.camera_pos = rotate_around(v.camera_pos, v.lookat, axis, angle);
}

//orbit or fly rotation movement
static void	rotate_camera()
{	
	double dist_to = vec_dist(v.camera_pos, v.lookat);

	int one = 1 - 2*v.cam_flipp;

	if (v._np0)
	{
		v.camera_pos = v3(0, 0, 0);
		v.camera_rot = lookRotation(v3(), v3(0, 0, -1));
		v.lookat_toggle = 0;
	}
	if (v._np1) // from Z
	{
		v.camera_pos = v3(0, 0, dist_to);
		v.lookat_toggle = 1;
	}
	if (v._np3) // from X
	{
		v.camera_pos = v3(dist_to);
		v.lookat_toggle = 1;
	}
	if (v._np7) // from Y
	{
		v.camera_pos = v3(0, dist_to);
		v.lookat_toggle = 1;
	}
	
	if (v._np5)
	{
		v.orthographic_toggle = !v.orthographic_toggle;
		v._rerender = 1;
	}

	if (v.lookat_toggle)
	{
		if (v._np9) NOT_IMPLEMENTED("Flip around Y axis")

		if (v._np4) do_orbit_move(v3(0, one), 15*DEG2RAD);
		if (v._np6)	do_orbit_move(v3(0,-one), 15*DEG2RAD);
		if (v._np2) do_orbit_move(v3(-one), 15*DEG2RAD);
		if (v._np8) do_orbit_move(v3( one), 15*DEG2RAD);
	}
	else /* look infront */
	{
		// if (v._np9) v.camera_rot.z += one*15*DEG2RAD;
		if (v._np4) v.camera_rot.y -= one*15*DEG2RAD;
		if (v._np6) v.camera_rot.y += one*15*DEG2RAD;
		if (v._np2) v.camera_rot.x += one*15*DEG2RAD;
		if (v._np8) v.camera_rot.x -= one*15*DEG2RAD;
	}
}

static void	move_camera()
{
	double move_speed = get_move_speed();
	double angular_speed = get_angular_speed();

	int one = 1 - 2*v.cam_flipp;

	if (v.lookat_toggle)
	{
		vec3 move;
		if (v._q)	do_orbit_move(v3( one), angular_speed*v.delta_time);
		if (v._e)	do_orbit_move(v3(-one), angular_speed*v.delta_time);

		if (v._right)				do_orbit_move(v3(0,-one), angular_speed*v.delta_time);
		if (v._left)				do_orbit_move(v3(0, one), angular_speed*v.delta_time);

		if (v._up || v._down)
		{
			if (v._up)		move = v3(0, 0, 1);
			if (v._down)	move = v3(0, 0,-1);

			move = v_scal(move, angular_speed*v.delta_time);

			v.camera_pos = v_add(v.camera_pos, rotate3(move, v.camera_rot));
		}
	}
	else
	{
		vec3 move;
		if (v._right)				move = v3( one);
		if (v._left)				move = v3(-one);
		if (v._up)					move = v3(0, 0, 1);
		if (v._down)				move = v3(0, 0,-1);
		if (v._q)	move = v3(0, one);
		if (v._e)	move = v3(0,-one);

		move = v_scal(move, move_speed*v.delta_time);

		v.camera_pos = v_add(v.camera_pos, rotate3(move, v.camera_rot));
	}
}

static void	fov_camera()
{
	if (v._scroll)
		v.vfov = 50 + v._scroll;
}
