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

# define SAMPLES 10
static int samples = 1;
static int steps_rendered = 0;
const int  samples_per_step = 10;


void	init_ray(double x, double y, ray *r)
{
	//Only sample when you want to look at, also sample >1
	if (v.lookat_toggle)
	{
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
	}

	vec3 d;
	d = get_ray_direction(x, y);
	d = rotate3(d, v.camera_rot);
	r->orig = v.camera_center;
	r->dir = v_norm(d);
}

vec3	ray_color(ray *r, int depth)
{
	hit_record rec;
	
	if (depth <= 0)
		return BLACK;
	
	if (!hit(r, (interval){0.001, INFINITY}, &rec))
	{
		vec3 uv = v_scal(v_add(r->dir, v_3(1)), 0.5);
		return v.background_color(uv);
	}

	if (v.render_mode == RAYTRACE_UVS)
		return new_color(rec.u-((int)rec.u/1), rec.v-((int)rec.v/1), 0);

	ray	scattered;
	color attenuation;
	color color_from_emission = rec.mat.emissive.value(rec.u, rec.v, &rec.mat.emissive);
	if (!rec.mat.scatter(r, &rec, &attenuation, &scattered, &(rec.mat)))
		return color_from_emission;
	
	color color_from_scatter = v_mult(ray_color(&scattered, depth-1), attenuation);

	return v_add(color_from_scatter, color_from_emission);
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

	vec3	pixel_color = BLACK;
	for (int i=0; i<samples; i++)
	{
		ray r;
		init_ray(x, y, &r);

		pixel_color = v_add(pixel_color, ray_color(&r, 50)); // MAX_DEPTH
	}
	
	if (v.render_mode == RAYTRACE || v.render_mode == RAYTRACE_UVS || v.render_mode == RENDER_MOVIE)
		pixel_color = v_scal(pixel_color, 1.0/samples);
	else if (v.render_mode == RAYTRACE_STEPS)
	{
		color from = rgb2color(get_pixel(x, y));
		from = v_mult(from, from);
		from = v_scal(from, steps_rendered*samples_per_step);
		pixel_color = v_scal(v_add(from, pixel_color), 1.0/((steps_rendered + 1.0) * samples_per_step));
	}
	draw_gamma_corrected(x, y, pixel_color);
	
	if (UPSCALE > 1)
	{
		for (int _y=0; _y<UPSCALE; _y++)
		{
			for (int _x=0; _x<UPSCALE; _x++)
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
				_x+=UPSCALE;
				if (v.render_mode != RENDER_MOVIE && get_elapsed(v.last_update) > 1000000.0/60.0)
				{
					print_progress(_split*w_size*h_size + _x + _y*w_size);
					return;
				}
			}
			_x = 0;
			_y+=UPSCALE;
		}
		_y = 0;
		_split++;
		if (_split == w_splits*h_splits)
		{
			print_progress(v.w * v.h);
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
