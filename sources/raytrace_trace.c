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

static void	trace_sub_hit(color *light, color *contribution, hit_record *rec,
		ray *r)
{
	color		emitted_light;
	ray			scattered;
	shader_end	se;
	bool		was_specular;

	emitted_light = evaluate(&(rec->mat.emission), rec->u, rec->v);
	emitted_light = v_scal(emitted_light, rec->mat.emission_strength);
	*light = v_add(*light, v_mult(emitted_light, *contribution));
	se = pbr_lighting(rec, r);
	pbr_scatter(r, rec, &scattered, &was_specular);
	*light = v_add(*light, v_mult(paint_se(&se), *contribution));
	if (was_specular)
		*contribution = v_mult(*contribution, se.specular_color);
	else
		*contribution = v_mult(*contribution, se.diffuse_color);
	*r = scattered;
}

static bool	trase_sub_hitnt(color *light, color *c, ray *r, int bounce)
{
	vec3	uv;
	vec3	bg_light;

	if (v.render_mode != RAYTRACE_MAT_DEBUG)
	{
		if (bounce == 0 && v.use_ibl)
			return (*light = paint_env(r->dir), 1);
		uv = v_scal(v_add(r->dir, v_3(1)), 0.5);
		if (v.use_ibl)
			bg_light = paint_env(r->dir);
		else
			bg_light = v_mult(evaluate(&v.background, uv.x, uv.y), *c);
		*light = v_add(*light, bg_light);
		return (0);
	}
	else
	{
		if (v.mat_debugmode == COMBINED)
			*light = paint_env(r->dir);
		else
			*light = BLACK;
		return (1);
	}
}

static bool	other_rendermodes(color *light, bool d_hit, hit_record *rec, ray *r)
{
	if (v.render_mode == RAYTRACE_UVS)
		*light = paint_uv(d_hit, rec);
	else if (v.render_mode == RAYTRACE_MAT_DEBUG && is_unlit(v.mat_debugmode))
		*light = paint_mat_debug_unlit(d_hit, rec, r);
	else if (v.render_mode == RAYTRACE_MAT_VALUES)
		*light = mat_values(d_hit, rec);
	else if (v.render_mode == RAYTRACE_MAT_DEBUG && d_hit)
		*light = paint_mat_debug_lit(r, rec);
	return (v.render_mode == RAYTRACE_UVS
		|| (v.render_mode == RAYTRACE_MAT_DEBUG && is_unlit(v.mat_debugmode))
		|| v.render_mode == RAYTRACE_MAT_VALUES
		|| (v.render_mode == RAYTRACE_MAT_DEBUG && d_hit));
}

// aces tone
// gamma correct
color	tone(color light)
{
	if (v.tone_mode == RAW)
		return (light);
	else if (v.tone_mode == GAMMA)
		return (gamma_correct(light));
	else if (v.tone_mode == ACES)
		return (aces_tone(light));
	else if (v.tone_mode == HDR)
		return (hdr_tone(light));
	else if (v.tone_mode == ACES_GAMMA)
		return (gamma_correct(aces_tone(light)));
	else if (v.tone_mode == HDR_GAMMA)
		return (gamma_correct(hdr_tone(light)));
	else if (v.tone_mode == ACES_HDR_GAMMA)
		return (gamma_correct(hdr_tone(aces_tone(light))));
	else if (v.tone_mode == HDR_ACES_GAMMA)
		return (gamma_correct(aces_tone(hdr_tone(light))));
	return (ERROR_CYAN);
}

color	trace(ray *r, int max_depth)
{
	color		light;
	color		contribution;
	hit_record	rec;
	bool		did_hit;
	int			bounce;

	light = BLACK;
	contribution = WHITE;
	bounce = 0;
	while (bounce <= max_depth)
	{
		did_hit = hit(r, (interval){0.001, INFINITY}, &rec);
		if (other_rendermodes(&light, did_hit, &rec, r))
			return (light);
		if (did_hit)
			trace_sub_hit(&light, &contribution, &rec, r);
		else
		{
			if (trase_sub_hitnt(&light, &contribution, r, bounce))
				return (light);
			break ;
		}
		bounce++;
	}
	return (tone(light));
}
