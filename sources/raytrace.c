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

#define W_SPLITS 7
#define H_SPLITS 11

static void	reset_render(int *_split, struct timeval *frame_start)
{
	v._rerender = 0;
	if (v._debug)
		(print_pos(v.camera_pos, "from: "), print_pos(v.camera_rot, "rot : "));
	if (!v.rendering_movie)
	{
		(clear_img(v.img), reset_heatmap(), raster_items(), raster_lamps());
	}
	memset(v.accumulate_img[0], 0, v.w * v.h * sizeof(vec3));
	v.samples = v.max_samples;
	if (v.render_mode == RAYTRACE_STEPS)
		v.samples = v.samples_per_step;
	else if (v.render_mode == RAYTRACE_UVS
		|| v.render_mode == RAYTRACE_MAT_DEBUG
		|| v.render_mode == RAYTRACE_MAT_VALUES)
		v.samples = 1;
	v.steps_rendered = 0;
	*_split = 0;
	gettimeofday(frame_start, 0);
}

void	raytrace(void)
{
	static t_split_task		tasks[W_SPLITS * H_SPLITS];
	static struct timeval	frame_start;
	static int				_split = 0;

	if (v._rerender || v.rendering_movie)
	{
		reset_render(&_split, &frame_start);
		generate_tasks(tasks, W_SPLITS, H_SPLITS, v.upscale);
	}
	while (_split < W_SPLITS * H_SPLITS)
	{
		if (!fullfill(&tasks[_split], _split))
			return ;
		_split++;
		show_progress(_split, W_SPLITS * H_SPLITS, frame_start);
	}
	if (v.render_mode == RAYTRACE_STEPS)
	{
		_split = 0;
		v.steps_rendered += 1;
		gettimeofday(&frame_start, 0);
	}
}
