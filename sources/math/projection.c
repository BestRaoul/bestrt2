
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


vec3	world_to_viewport(vec3 cam_pos, PFPN vfov,
		PFPN aspect, vec3 pos)
{
	vec3	p;

	p = world_to_local(cam_pos, pos);
	p = project(p, vfov, aspect);
	return (clip_space_to_viewport(p));
}

vec3	world_to_screenpos(vec3 pos)
{
	vec3	p;

	p = world_to_viewport(v.camera_pos, v.vfov, (PFPN)v.w
			/ (PFPN)v.h, pos);
	p.x *= v.w;
	p.y *= v.h;
	p.y = v.h - p.y;
	if (v.cam_flipp)
	{
		p.x = v.w - p.x;
		p.y = v.h - p.y;
	}
	return (p);
}

vec3	world_to_screenpos_fixed_fov(vec3 pos, PFPN fov)
{
	vec3	p;

	p = world_to_viewport(v.camera_pos, fov, (PFPN)v.w
			/ (PFPN)v.h, pos);
	p.x *= v.w;
	p.y *= v.h;
	p.y = v.h - p.y;
	if (v.cam_flipp)
	{
		p.x = v.w - p.x;
		p.y = v.h - p.y;
	}
	return (p);
}

vec3	world_to_screenpos_ortho(vec3 pos)
{
	vec3	p;

	p = world_to_local(v.camera_pos, pos);
	p = project_ortho(p, v.vfov, (PFPN)v.w / (PFPN)v.h);
	p = clip_space_to_viewport(p);
	p.x *= v.w;
	p.y *= v.h;
	p.y = v.h - p.y;
	if (v.cam_flipp)
	{
		p.x = v.w - p.x;
		p.y = v.h - p.y;
	}
	return (p);
}