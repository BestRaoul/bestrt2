/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raster.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/*
	d = get_ray_direction(x, y);
	d = rotate3(d, v.camera_rot);
	r->orig = v.camera_center;
	r->dir = v_norm(d);
*/

void	init_ray(PFPN x, PFPN y, ray *r)
{
	vec3	pixel_center;
	vec3	pixel_sample;
	vec3	ray_direction;
	vec3	d;

	x = v.w - x;
	pixel_center = v_add(v_add(v.pixel00_loc, v_scal(v.pixel_delta_u, x)),
			v_scal(v.pixel_delta_v, y));
	pixel_sample = v_add(pixel_center, v_scal(pixel_sample_square(),
				(v.samples > 1 || v.steps_rendered > 0)));
	ray_direction = v_sub(pixel_sample, v.camera_center);
	if (v.orthographic_toggle)
	{
		r->orig = v_scal(pixel_sample, vec_dist(v.camera_pos, v3()));
		r->dir = rotate3(v3(0, 0, -1), v.camera_rot);
		return ;
	}
	if (v.defocus_angle == 0)
		r->orig = v.camera_center;
	else
		r->orig = defocus_disk_sample();
	r->dir = v_norm(ray_direction);
}

#define HALF 0
#define SPEEDUP 0

bool	is_eliminated(int x, int y)
{
	if (HALF)
	{
		if (x % 3)
			return (True);
	}
	if (SPEEDUP)
	{
		if (x % 2 && (y + 1) % 2)
			return (True);
		if ((x + 1) % 2 && y % 2)
			return (True);
	}
	return (False);
}

void	draw_gamma_square(vec3 p, int w, int h, color c)
{
	int	_y;
	int	_x;

	_y = 0;
	while (_y < h)
	{
		_x = 0;
		while (_x < w)
		{
			draw_gamma_corrected(p.x + _x, p.y + _y, c);
			_x++;
		}
		_y++;
	}
}

void	render_pixel(int x, int y)
{
	color	pixel_color;
	color	sample;
	ray		r;
	int		i;

	if (is_eliminated(x, y))
		return ;
	x *= v.upscale;
	y *= v.upscale;
	sample = BLACK;
	i = 0;
	while (i < v.samples)
	{
		init_ray(x, y, &r);
		sample = v_add(sample, trace(&r, v.max_depth));
		i++;
	}
	v.accumulate_img[x][y] = v_add(v.accumulate_img[x][y], sample);
	pixel_color = v_scal(v.accumulate_img[x][y], 1.0 / (v.samples * (1
					+ v.steps_rendered * (v.render_mode == RAYTRACE_STEPS))));
	draw_gamma_square(v3(x, y), v.upscale, v.upscale, pixel_color);
}
