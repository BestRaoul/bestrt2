/*                                                        :::      ::::::::   */
/*   raytrace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

# define SAMPLES 30
static int samples = 1;
static int steps_rendered = 0;
const int  samples_per_step = 10;


void	init_ray(double x, double y, ray *r)
{
	//Only sample when you want to look at, also sample >1

	x = v.w-x;
	vec3 pixel_center = v_add(v_add(
		v.pixel00_loc, 
		v_scal(v.pixel_delta_u, x)),
		v_scal(v.pixel_delta_v, y));
	vec3 pixel_sample = v_add(pixel_center, pixel_sample_square());
	vec3 ray_direction = v_sub(pixel_sample, v.camera_center); // fix to center

	r->orig = v.camera_center;
	r->dir = v_norm(ray_direction);
	return;

/*
	vec3 d;
	d = get_ray_direction(x, y);
	d = rotate3(d, v.camera_rot);
	r->orig = v.camera_center;
	r->dir = v_norm(d);
*/
}

color	trace(ray *r, int max_depth)
{
	vec3	light = BLACK;
	color	contribution = WHITE;

	for (int i=0; i<max_depth; i++)
	{
		hit_record rec;

		if (hit(r, (interval){0.001, INFINITY}, &rec))
		{
			color 	emitted_light;
			color	material_color;
			ray		scattered;

			PBR_scatter(r, &rec, &emitted_light, &material_color, &scattered, &rec.mat);

			contribution = v_mult(contribution, material_color);
			light = v_add(light, emitted_light);

			*r = scattered;
		}
		else
		{
			vec3 uv = v_scal(v_add(r->dir, v_3(1)), 0.5);
			vec3 bg_light = v_mult(v.background_color(uv), contribution);
			light = v_add(light, bg_light); 
			break;
		}
	}
	return light;
}

# define SPEEDUP 0
void    render_pixel(int x, int y)
{
	//4x speedup
	if (SPEEDUP)
	{
		if (x % 2 && (y+1) % 2) return;
		if ((x+1) % 2 && y % 2) return;
		if (x % 3) return;
		if (y % 3) return;
	}



	color	sample = BLACK;
	for (int i=0; i<samples; i++)
	{
		ray r;
		init_ray(x, y, &r);

		sample = v_add(sample, trace(&r, v.max_depth));
	}
	v.accumulate_img[x][y] = v_add(v.accumulate_img[x][y], sample);
	//*accumulate = sample;//v_add(*accumulate, sample);

	int total_samples = v.render_mode == RAYTRACE_STEPS ? (steps_rendered + 1.0) * samples : samples;
	color pixel_color = v_scal(v.accumulate_img[x][y], 1.0/(double)total_samples);
	draw_gamma_corrected(x, y, pixel_color);
	
	if (v.upscale > 1)
	{
		for (int _y=0; _y<v.upscale; _y++)
		{
			for (int _x=0; _x<v.upscale; _x++)
			{
				draw_gamma_corrected(x+_x, y+_y, pixel_color);
			}
		}
	}

	if (SPEEDUP)
	{
		draw_gamma_corrected(x+1, y, pixel_color);
		draw_gamma_corrected(x, y+1, pixel_color);
		draw_gamma_corrected(x+1, y+1, pixel_color);
	}
}

void	print_progress(int pixel_count)
{
	double progress = (double)pixel_count / (v.w * v.h) * 100;
	write(1, "\33[2K\r", ft_strlen("\33[2K\r"));
	const char pm[21] 	 = "####################";
	const char no_pm[21] = "....................";
	char s[100];	sprintf(s, "frame %d : [%s%.*s%s%.*s]  %.0f%%", steps_rendered, T_GRN, (int)progress/5, pm, T_RESET, 20-(int)progress/5, no_pm, progress);
	write(1, s, ft_strlen(s));
}

void    raytrace(void)
{
	int	w_splits = 4;
	int	h_splits = 4;
	int			w_size = v.w/w_splits;
	int			h_size = v.h/h_splits;

	static int	_split = 0;
	static int	_x = 0;
	static int	_y = 0;

	static struct timeval frame_start;

	//static ;
	if (v._rerender)
	{
		printf("RERENDER ---\n");
		printf("see : %d\n", (int)v.vfov);
		print_pos(v.camera_pos, "from: ");
		print_pos(v_scal(v.camera_rot, RAD2DEG), "rot : ");
		_split = 0;
		_x = 0; _y = 0;
		v._rerender = 0;
		clear_img(v.img);
		if (v.render_mode == RAYTRACE || v.render_mode == RAYTRACE_UVS)
		{
			reset_heatmap();
			raster_items();
		}

		samples = SAMPLES;
		if (v.render_mode == RAYTRACE_STEPS)
			samples = samples_per_step;
		steps_rendered = 0;

		gettimeofday(&frame_start, 0);
	}

	while (_split < w_splits*h_splits)
	{
		int x_off = (_split%w_splits)*w_size;
		int y_off = (_split/w_splits)*h_size;
		
		while (_y< h_size)
		{
			while (_x < w_size)
			{
				render_pixel(_x+x_off, _y+y_off);
				_x+=v.upscale;
				if (v.render_mode != RENDER_MOVIE && get_elapsed(v.last_update) > 1000000.0/60.0)
				{
					print_progress(_split*w_size*h_size + _x + _y*w_size);
					return;
				}
			}
			_x = 0;
			_y+=v.upscale;
		}
		_y = 0;
		_split++;
		if (_split == w_splits*h_splits)
		{
			print_progress(v.w * v.h);
			printf(" in %.2f ms", get_elapsed(frame_start)/1000.0);
			gettimeofday(&frame_start, 0);
			printf("\n");
		}
		else if (v.render_mode == RENDER_MOVIE)
			print_progress(_split*w_size*h_size);
	}
	
	if (v.render_mode == RAYTRACE_STEPS)
	{
		_split = 0;
		_x = 0; _y = 0;
		steps_rendered+=1;
	}
}
