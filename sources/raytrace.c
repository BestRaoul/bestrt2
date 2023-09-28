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

static int samples = 1;
static int steps_rendered = 0;

void	init_ray(double x, double y, ray *r)
{
	//Only sample when you want to look at, also sample >1

	x = v.w-x;
	vec3 pixel_center = v_add(v_add(
		v.pixel00_loc, 
		v_scal(v.pixel_delta_u, x)),
		v_scal(v.pixel_delta_v, y));
	vec3 pixel_sample = v_add(pixel_center, 
		v_scal(pixel_sample_square(), samples > 1));
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

color	paint_dist(double d)
{
	if (d < 0)
		d = v.far_plane;
	
	//Linear fog
	//double f = (MAX_DIST - d) / (MAX_DIST);
	
	//Exponential fog
	double density = 0.03;
	//double f = (1.0) / (pow(2.7, d * density));
	//Exponential fog 2
	double f = (1.0) / (pow(pow(2.7, d * density), 2));
	return lerp(f, BLACK, WHITE);
	//return lerp(f, v3(.1, .3, .2), v3(.8, .2, .3));
}

color	material_debug(ray *r, hit_record *rec, ray *scattered)
{
	if (0) //Albedo
	{
		return evaluate(&rec->mat.base_color, rec->u, rec->v);
	}
	else if (0) //Metalic
	{
		return evaluate(&rec->mat.metalic, rec->u, rec->v);
	}
	else if (0) //Specular
	{
		return evaluate(&rec->mat.specular, rec->u, rec->v);
	}
	else if (0) //roughness
	{
		return evaluate(&rec->mat.roughness, rec->u, rec->v);
	}
	else if (0) //transmission_roughness
	{
		return evaluate(&rec->mat.transmission_roughness, rec->u, rec->v);
	}
	else if (1) //normal
	{
		return rec->normal;
	}
}

color	trace(ray *r, int max_depth)
{
	vec3	light = v3(0.02, 0.02, 0.02);
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

			//light = v_add(light, v_mult(emitted_light, contribution));
			light = v_add(light, v_mult(CalcTotalPBRLighting(&rec, r), contribution));
			contribution = v_mult(contribution, material_color);

			*r = scattered;

			if (v.render_mode == RAYTRACE_UVS)
				return evaluate(&v.uv_debug, rec.u, rec.v);//v3(rec.u, rec.v, 0); //
			if (v.render_mode == RAYTRACE_DIST)
				return paint_dist(rec.t);
			if (v.render_mode == RAYTRACE_MAT_DEBUG)
				return material_debug(r, &rec, &scattered);
		}
		else
		{
			vec3 uv = v_scal(v_add(r->dir, v_3(1)), 0.5);
			vec3 bg_light = v_mult(v.background_color(uv), contribution);
			light = v_add(light, bg_light); 
			if (v.render_mode == RAYTRACE_UVS)
				return uv; //evaluate(&v.uv_debug, uv.x, uv.y);//
			if (v.render_mode == RAYTRACE_DIST)
				return paint_dist(-1);
			if (v.render_mode == RAYTRACE_MAT_DEBUG)
				return uv;
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
		print_pos(v.camera_rot, "rot : ");
		_split = 0;
		_x = 0; _y = 0;
		v._rerender = 0;
		clear_img(v.img);
		if (v.render_mode == RAYTRACE
		 || v.render_mode == RAYTRACE_UVS
		 || v.render_mode == RAYTRACE_DIST)
		{
			reset_heatmap();
			raster_items();
		}
		memset(v.accumulate_img[0], 0, sizeof(vec3) * v.w * v.h);

		samples = v.max_samples;
		if (v.render_mode == RAYTRACE_STEPS)
			samples = v.samples_per_step;
		else if (v.render_mode == RAYTRACE_UVS || v.render_mode == RAYTRACE_DIST || v.render_mode == RAYTRACE_MAT_DEBUG)
			samples = 1;
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
