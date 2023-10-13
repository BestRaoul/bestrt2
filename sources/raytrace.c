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
	vec3 ray_direction = v_sub(pixel_sample, v.camera_center);
	if (v.orthographic_toggle)
	{
		r->orig = v_scal(pixel_sample, vec_dist(v.camera_pos, v3()));
		r->dir = rotate3(v3(0, 0, -1), v.camera_rot);
		return;
	}

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

color	paint_dist(Bool did_hit, double d)
{
	if (d < 0 || !did_hit)
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

color	paint_uv(Bool did_hit, hit_record *rec)
{
	if (did_hit)
		return evaluate(&v.uv_debug, rec->u, rec->v);
	return BLACK; // v_scal(v_add(r->dir, v_3(1)), 0.5);
}

color	paint_env(vec3 r_dir)
{
	vec3 uv = v_scal(v_add(r_dir, v_3(1)), 0.5);
	color irradiance = evaluate(&v.irradiance_map, uv.x, uv.y);
	return irradiance;			
}

//see material values
color	show_mat_values(hit_record *rec)
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

color	paint_se(shader_end *se)
{
	return v_add(v_mult(se->specular_color, se->specular_light),
				 v_mult(se->diffuse_color, se->diffuse_light));
}

# define UNLIT(mode) (mode==NORMAL || mode==MIST || mode==EMISSION || mode==ENVIRONEMENT)

color	paint_mat_debug_unlit(Bool did_hit, hit_record *rec, ray *r)
{
	if (v.mat_debugmode==NORMAL)
	{
		if (!did_hit) return BLACK;
		maybe_apply_perturb(rec);
		double r = -rec->normal.x;
		double g = -rec->normal.z;
		double b = rec->normal.y;
		return v3(r,g,b);
	}
	else if (v.mat_debugmode==MIST)
	{
		return paint_dist(did_hit, rec->t);
	}
	else if (v.mat_debugmode==EMISSION)
	{
		if (!did_hit) return BLACK;
		color e = evaluate(&rec->mat.emission, rec->u, rec->v);
		e = v_scal(e, rec->mat.emission_strength);
		return e;
	}
	else if (v.mat_debugmode==ENVIRONEMENT)
	{
		if (did_hit) return BLACK;
		return paint_env(r->dir);
	}
	return ERROR_CYAN;
}

color	paint_mat_debug_lit(shader_end *se, hit_record *rec)
{
	if (v.mat_debugmode==DIFFUSE_LIGHT)
	{
		return se->diffuse_light;
	}
	else if (v.mat_debugmode==DIFFUSE_COLOR)
	{
		return se->diffuse_color;
	}
	else if (v.mat_debugmode==SPECULAR_LIGHT)
	{
		return se->specular_light;
	}
	else if (v.mat_debugmode==SPECULAR_COLOR)
	{
		return se->specular_color;
	}
	else if (v.mat_debugmode==COMBINED)
	{
		return paint_se(se);
	}
	write(1, "error", 5);
	return ERROR_CYAN;
}


void	maybe_apply_perturb(hit_record *rec)
{
	vec3 normalRGB = evaluate(&rec->mat.normal, rec->u, rec->v);
	vec3 perturbation = v3(normalRGB.x*2 -1, normalRGB.y*2 -1, normalRGB.z);
	perturbation = v_scal(perturbation, rec->mat.normal_strength);
	perturbation.z = fabs(perturbation.z);
	vec3 new_normal = perturb_normal(rec->normal, perturbation);
	rec->old_normal = rec->normal;
	rec->normal = new_normal;
}

color	trace(ray *r, int max_depth)
{
	vec3	light = BLACK;
	color	contribution = WHITE;

	for (int bounce=0; bounce<=max_depth; bounce++)
	{
		hit_record rec;
		Bool did_hit = hit(r, (interval){0.001, INFINITY}, &rec);

		if		(v.render_mode == RAYTRACE_UVS) return paint_uv(did_hit, &rec);
		else if	(v.render_mode == RAYTRACE_DIST) return paint_dist(did_hit, rec.t);
		else if	(v.render_mode == RAYTRACE_MAT_DEBUG && UNLIT(v.mat_debugmode)) return paint_mat_debug_unlit(did_hit, &rec, r);

		if (did_hit)
		{
			color 	emitted_light;
			ray		scattered;

			emitted_light = evaluate(&(rec.mat.emission), rec.u, rec.v);
			emitted_light = v_scal(emitted_light, rec.mat.emission_strength);
			light = v_add(light, v_mult(emitted_light, contribution));
			
			maybe_apply_perturb(&rec);
    		
			shader_end se = CalcTotalPBRLighting(&rec, r);
			if (v.render_mode == RAYTRACE_MAT_DEBUG) return paint_mat_debug_lit(&se, &rec);
			
			Bool was_specular;
			PBR_scatter(r, &rec, &scattered, &was_specular);

			light = v_add(light, v_mult(paint_se(&se), contribution));

			contribution = v_mult(contribution, was_specular?se.specular_color:se.diffuse_color);

			*r = scattered;
		}
		else
		{
			if (v.render_mode != RAYTRACE_MAT_DEBUG)
			{
				if (bounce==0 && v.background_color == NULL)return paint_env(r->dir);

				vec3 uv = v_scal(v_add(r->dir, v_3(1)), 0.5);
				vec3 bg_light = v_mult(v.background_color(uv), contribution);
				light = v_add(light, bg_light);
				break;
			}		
			else return v.mat_debugmode==COMBINED?paint_env(r->dir):BLACK;
			/*
			if (v.render_mode == RAYTRACE_STEPS) break;
			break;
	
			vec3 bg_light = v_mult(paint_env(r->dir), contribution);//v.background_color(uv), contribution);
			light = v_add(light, bg_light); 	
			break;
			*/
		}
	}
	//light = hdr_tone(light);
	//light = gamma_correct(light);
	return light;
}

# define HALF 1
# define SPEEDUP 0
void    render_pixel(int x, int y)
{
	if (HALF)
	{
		if (x % 2) return;
	}
	//4x speedup
	if (SPEEDUP)
	{
		// if (x % 2 && (y+1) % 2) return;
		// if ((x+1) % 2 && y % 2) return;
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

	int total_samples = (v.render_mode == RAYTRACE_STEPS
					  || v.render_mode == RAYTRACE_STEPS_2
					  || v.render_mode == RAYTRACE_STEPS_3) ? (steps_rendered + 1.0) * samples : samples;
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
		if (v.render_mode == RAYTRACE_STEPS
	 	 || v.render_mode == RAYTRACE_STEPS_2
	 	 || v.render_mode == RAYTRACE_STEPS_3)
			samples = v.samples_per_step;
		else if (v.render_mode == RAYTRACE_UVS || v.render_mode == RAYTRACE_DIST || v.render_mode == RAYTRACE_MAT_DEBUG)
			samples = 1;
		steps_rendered = 0;

		gettimeofday(&frame_start, 0);

		t_item *item;
		for (int i=0; i<v.item_count; i++)
		{
			item = &v.items[i];
			transform t = (transform){item->pos, item->rot, item->scale};
			set_transform_matrix(&t, item->fwd, item->bck);
		}
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
				if (!v.rendering_movie && get_elapsed(v.last_update) > 1000000.0/60.0)
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
	
	if (v.render_mode == RAYTRACE_STEPS
	 || v.render_mode == RAYTRACE_STEPS_2
	 || v.render_mode == RAYTRACE_STEPS_3)
	{
		_split = 0;
		_x = 0; _y = 0;
		steps_rendered+=1;
	}
}
