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
static void _render(void);

int	loop(void)
{
	_update();
	_render();
	v.frame++;
	v.time_passed += v.delta_time * v.time_speed;
	_reset_consumable_clicks();
	return (0);
}

static void apply_motions()
{
	if (!v.motion_enabled) return; //fix so it only adds time when enabled
	
	double t = v.time_passed - (int)v.time_passed/1;
	
	for(int i=0; i<v.motion_count; i++)
	{
		motion m = v.motions[i];
		*m.value = m.tween(m.start_val, m.end_val, t);
	}
}

static void	_update(void)
{
	update_delta_time();
	
	apply_motions();

	update_camera();

	maybe_add_item();
	manage_selection();

	//if (_rerender)
	//set_all_transform_matrices()
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

void	render_movie()
{
	//close window
	mlx_destroy_window(v.mlx, v.win);
	
	v.render_mode = v.animation_render_mode;
	v.rendering_movie = True;

	int	framerate = v.animation_framerate;
	int	frames = framerate * v.animation_duration;
	v.time_speed = v.animation_speed;
	int loops = v.animation_loops;
	//Fix delta time
	v.delta_time = 1.0/framerate;
	//Reset motions
	v.time_passed = 0;
	v.motion_enabled = 1;
	
	for (int i=0; i<frames; i++)
	{
		//update scene
		apply_motions();
		update_camera();

		//generate img, but dont show it
		v._rerender = 1;
		raytrace();

		//write img to disk
		write_img();

		v.frame++;
		v.time_passed += v.delta_time * v.time_speed;

		printf("%d/%d !\n", i, frames);
	}

	ffmpeg_bmp_to_mp4(framerate, loops);
	//ffmpeg should fork();...
	//?destroy bmp
	exit(0);
}

/*
static void	draw_heatmap_to_img()
{
	for (int x=0; x<v.w; x++)
		for (int y=0; y<v.h; y++)
			{
				double dist = v.dist_heatmap[x][y];
				int farfar = 15;
				int l = (farfar*farfar - dist*dist)/farfar/farfar;
				color c = new_color(.25, l*(dist<farfar), 0);
				draw_raw(x, y, c);
			}
}

// else if (v.render_mode == RASTER_HEATMAP); //raster(); draw_heatmap_to_img();
*/