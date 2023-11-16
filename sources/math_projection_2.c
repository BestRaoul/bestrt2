/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_projection_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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
