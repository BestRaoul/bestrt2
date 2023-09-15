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

static int	rotate_camera();
static int	move_camera();
static void set_camera_values();
mat4	GetOrientation(void);

void    update_camera(void)
{
    int	change = 0;
	change |= move_camera();
    change |= rotate_camera();
    if (v.frame == 0 || change || v._p)
	{
		set_camera_values();
		v._rerender = 1;
		v.rotation_matrix = GetOrientation();
	}
}

quat	angleAxis(float angle, vec3 axis) {
    float s = sin(angle / 2);
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

mat4	GetOrientation(void)
{
    quat q = angleAxis(v.camera_rot.x,v3(1,0,0));
    quat q2 = angleAxis(v.camera_rot.y, v3(0,1,0));
	quat q3 = angleAxis(v.camera_rot.z, v3(0,0,1));
    q = quaternion_multiply(q2, q);
	q = quaternion_multiply(q3, q);
	return quaternion_to_mat4(q);
}

static void set_camera_values()
{
	double	aspect_ratio = (double)v.w/(double)v.h;

	v.lookfrom = v.camera_pos;

	//...

	vec3	_u, _v, _w;

	//initialize
	v.camera_center = v.lookfrom;
	double focal_length = vec_dist(v.lookfrom, v.lookat);
	double theta = v.vfov * DEG2RAD;
	double h = tan(theta/2);
	double viewport_height = 2 * h * focal_length;
	double viewport_width = viewport_height * aspect_ratio;

	//Calculate the u,v,w unit basis vectors for the camera coordinate frame.
	_w = v_norm(v_sub(v.lookfrom, v.lookat));
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

static int	rotate_camera()
{
	if (v._np9)
	{
		v.camera_pos = rotate_y(v.camera_pos, 15*DEG2RAD);
	}
	
	if (v._np5)
	{
		if (!v._ctrl)
			v.orthographic_toggle = !v.orthographic_toggle;
		else
			v.camera_rot.z += 15*DEG2RAD;
	}
	if (v._np0)
	{
		v.camera_pos = v3(0, 0, 0);
		v.camera_rot = v3(0, MYPI, 0);
	}
	if (v._np1) // from Z
	{
		v.camera_pos = v3(0, 0, -10);
		v.camera_rot = v3(0);
	}
	if (v._np3) // from X
	{
		v.camera_pos = v3(10);
		v.camera_rot = v3(0, -MYPI/2);
	}
	if (v._np7) // from Y
	{
		v.camera_pos = v3(0, 10);
		v.camera_rot = v3(MYPI/2, 0, 0);
	}
	

	if (v._np4)
		v.camera_rot.y -= 15*DEG2RAD;
		//v.camera_pos = rotate_y(v.camera_pos, 15 * DEG2RAD);
	if (v._np6)
		v.camera_rot.y += 15*DEG2RAD;
		//v.camera_pos = rotate_y(v.camera_pos, -15 * DEG2RAD);
	if (v._np2)
		v.camera_rot.x += 15*DEG2RAD;
	if (v._np8)
		v.camera_rot.x -= 15*DEG2RAD;

	if (v.lookat_toggle)
	{
		v.camera_rot = lookRotation(v.camera_pos, v.lookat);
	}

	return v._np0 || v._np1 || v._np2 || v._np3
		|| v._np4 || v._np5 || v._np6 || v._np7
		|| v._np8 || v._np9;
}

static int	move_camera()
{
	static int _prev_fov = 0;
	
	if (v._right)
		v.camera_pos = v_add(v.camera_pos, rotate3(v3(5*v.delta_time), v.camera_rot));
	if (v._left)
		v.camera_pos = v_add(v.camera_pos, rotate3(v3(-5*v.delta_time), v.camera_rot));
	if (v._up)
		v.camera_pos = v_add(v.camera_pos, rotate3(v3(0, 0, 5*v.delta_time), v.camera_rot));
	if (v._down)
		v.camera_pos = v_add(v.camera_pos, rotate3(v3(0, 0, -5*v.delta_time), v.camera_rot));
	if (v._space && !v._shift)
		v.camera_pos.y += 5*v.delta_time;
	if (v._space && v._shift)
		v.camera_pos.y -= 5*v.delta_time;
	//v.camera_pos.x += 5*v.delta_time;
	
	if (v._scroll)
		v.vfov = FOV + v._scroll;
	int	fov_changed = v.vfov != _prev_fov;
	if (fov_changed) _prev_fov = v.vfov;

	return v._right || v._left || v._up
		|| v._down || v._space || fov_changed;
}
