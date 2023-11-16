/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	_update(void);
static void	_render(void);

int	loop(void)
{
	_update();
	_render();
	v.time_passed += v.delta_time * v.time_speed;
	_reset_consumable_clicks();
	return (0);
}

static void	_update(void)
{
	update_delta_time();
	apply_motions();
	update_camera();
	v._rerender |= v.render_mode == RASTER;
	if (v._rerender)
		clear_img(v.img);
	maybe_add_item();
	manage_selection();
	update_lamp_stats();
	update_tfm_matrices();
}

static void	_render(void)
{
	if (v.render_mode == RASTER)
		raster();
	else
		raytrace();
	simulate_rayzz();
	mlx_put_image_to_window(v.mlx, v.win, v.img.mlx_img, 0, 0);
	help_ui();
	debug_ui();
}

// ffmpeg should fork();...
//?destroy bmp
//ffmpeg only in range from frame_start to frame_end
void	render_movie(void)
{
	int	framerate;
	int	frames;
	int	loops;
	int	frame_id;

	mlx_destroy_window(v.mlx, v.win);
	v.render_mode = v.animation_render_mode;
	v.rendering_movie = True;
	framerate = v.animation_framerate;
	frames = framerate * v.animation_duration;
	loops = v.animation_loops;
	v.delta_time = 1.0 / framerate;
	v.time_passed = 0;
	v.motion_enabled = 1;
	frame_id = 0;
	while (frame_id < frames)
	{
		(apply_motions(), update_camera(), raytrace(), write_img());
		printf("%d/%d !\n", frame_id++, frames);
		v.time_passed += v.delta_time * v.animation_speed;
	}
	ffmpeg_bmp_to_mp4(framerate, loops);
	exit(0);
}

/*
static void	draw_heatmap_to_img(void)
{
	PFPN	dist;
	int		farfar;
	int		l;
	color	c;

	for (int x=0; x<v.w; x++)
		for (int y=0; y<v.h; y++)
			{
				dist = v.dist_heatmap[x][y];
				farfar = 15;
				l = (farfar*farfar - dist*dist)/farfar/farfar;
				c = new_color(.25, l*(dist<farfar), 0);
				draw_raw(x, y, c);
			}
}

// else if (v.render_mode == RASTER_HEATMAP); //raster(); draw_heatmap_to_img();
*/