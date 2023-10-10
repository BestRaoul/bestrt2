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

mat4	quaternion_to_mat4(quat q) {
    mat4 m;
    m.m[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
    m.m[0][1] = 2 * q.x * q.y - 2 * q.z * q.w;
    m.m[0][2] = 2 * q.x * q.z + 2 * q.y * q.w;
    m.m[0][3] = 0;
    m.m[1][0] = 2 * q.x * q.y + 2 * q.z * q.w;
    m.m[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
    m.m[1][2] = 2 * q.y * q.z - 2 * q.x * q.w;
    m.m[1][3] = 0;
    m.m[2][0] = 2 * q.x * q.z - 2 * q.y * q.w;
    m.m[2][1] = 2 * q.y * q.z + 2 * q.x * q.w;
    m.m[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
    m.m[2][3] = 0;
    m.m[3][0] = 0;
    m.m[3][1] = 0;
    m.m[3][2] = 0;
    m.m[3][3] = 1;

    return m;
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
		NOT_IMPLEMENTED("CAMERA FLIPP");
		//v.vup = v_scal(v.vup, -1);
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
	quat q3 = angleAxis(v.camera_rot.z, v3(0,0,1));
    q = quaternion_multiply(q2, q);
	//q = quaternion_multiply(q3, q);
	v.rotation_matrix = quaternion_to_mat4(q);
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
	double focal_length = vec_dist(v.lookfrom, temp_lookat);
	double theta = v.vfov * DEG2RAD;
	double h = tan(theta/2);
	double viewport_height = 2 * h * focal_length;
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
		v_scal(_w, focal_length)),
		v_scal(viewport_u, .5)),
		v_scal(viewport_v, .5));
	v.pixel00_loc = v_add(viewport_upper_left,
		v_scal(v_add(v.pixel_delta_u, v.pixel_delta_v), .5) );
}

// -----

static void	rotate_camera()
{
	double dist_to = vec_dist(v.camera_pos, v.lookat);
	
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
		vec3 p = v_sub(v.camera_pos, v.lookat);
		double d = vec_dist(v.camera_pos, v.lookat);

		if (v._np0)
		{
			p = v3(0, 0, 0);
			v.camera_rot = lookRotation(v3(), v3(0, 0, -1));
			v.lookat_toggle = 0;
		}
		if (v._np1) // from Z
		{
			p = v3(0, 0, d);
			v.lookat_toggle = 1;
		}
		if (v._np3) // from X
		{
			p = v3(d);
			v.lookat_toggle = 1;
		}
		if (v._np7) // from Y
		{
			p = v3(0, d);
			v.lookat_toggle = 1;
		}

		if (v._np9) p = rotate_y(p, 180 * DEG2RAD);
		if (v._np4) p = rotate_y(p, 15 * DEG2RAD);
		if (v._np6)	p = rotate_y(p, -15 * DEG2RAD);
		if (v._np2)
		{
			double alpha = atan2(p.x, p.z);
			p = rotate_y(p, -alpha);
			p = rotate_x(p, 15 * DEG2RAD);
			p = rotate_y(p, alpha);
		}
		if (v._np8)
		{
			double alpha = atan2(p.x, p.z);
			p = rotate_y(p, -alpha);
			p = rotate_x(p, -15 * DEG2RAD);
			p = rotate_y(p, alpha);
		}

		v.camera_pos = v_add(p, v.lookat);
	}
	else /* look infront */
	{
		if (v._np9)
			v.camera_rot.z += 15*DEG2RAD;
		if (v._np4)
			v.camera_rot.y -= 15*DEG2RAD;
		if (v._np6)
			v.camera_rot.y += 15*DEG2RAD;
		if (v._np2)
			v.camera_rot.x += 15*DEG2RAD;
		if (v._np8)
			v.camera_rot.x -= 15*DEG2RAD;
	}
}

static void	move_camera()
{
	if (v._right)
		v.camera_pos = v_add(v.camera_pos, rotate3(v3(5*v.delta_time), v.camera_rot));
	if (v._left)
		v.camera_pos = v_add(v.camera_pos, rotate3(v3(-5*v.delta_time), v.camera_rot));
	if (v._up)
		v.camera_pos = v_add(v.camera_pos, rotate3(v3(0, 0, 5*v.delta_time), v.camera_rot));
	if (v._down)
		v.camera_pos = v_add(v.camera_pos, rotate3(v3(0, 0, -5*v.delta_time), v.camera_rot));
	if (v._space && !v._shift)
		v.camera_pos = v_add(v.camera_pos, rotate3(v3(0, 5*v.delta_time), v.camera_rot));
	if (v._space && v._shift)
		v.camera_pos = v_add(v.camera_pos, rotate3(v3(0, -5*v.delta_time), v.camera_rot));
}

static void	fov_camera()
{
	if (v._scroll)
		v.vfov = 50 + v._scroll;
}
