/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upate_item.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

vec3	dir_to_rot(vec3 dir)
{
	if (v_eq(dir, v3(0, 0, 0)))
		return (v3(0, MYPI, 0));
	dir = v_norm(dir);
	return (look_rotation(v3(), dir));
}

int	parse_camera(const char **line)
{
	v.camera_pos = atov3(*(line++), v.camera_pos);
	v.camera_rot = dir_to_rot(atov3(*(line++), v3(0, 0, -1)));
	v.vfov = atod(*(line++), v.vfov);
	return (1);
}

int	parse_camera_extension(const char **line)
{
	v.lookat_toggle = atod(*(line++), 0);
	v.lookat = atov3(*(line++), v3(0, 0, 0));
	v.defocus_angle = atod(*(line++), 0);
	v.focus_dist = atod(*(line++), 10);
	v.uv_debug = atotexture(*(line++), v.uv_debug);
	v.solo_lighting = atod(*(line++), 0);
	return (1);
}

int	parse_render_settings(const char **line)
{
	v.w = atod(*(line++), 1280);
	v.h = atod(*(line++), 720);
	v.upscale = atod(*(line++), 3);
	v.max_depth = atod(*(line++), 6);
	v.max_samples = atod(*(line++), 10);
	v.samples_per_step = atod(*(line++), 5);
	return (1);
}

int	parse_animation_settings(const char **line)
{
	v.animation_duration = atod(*(line++), 1.0);
	v.animation_framerate = atod(*(line++), 24);
	v.animation_speed = atod(*(line++), 1.0);
	v.animation_render_mode = atod(*(line++), RAYTRACE);
	v.animation_loops = atod(*(line++), 0);
	v.rendering_movie = False;
	return (1);
}
