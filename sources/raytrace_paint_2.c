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

Bool	is_unlit(int mode)
{
	return (mode == NORMAL
		|| mode == MIST
		|| mode == EMISSION
		|| mode == ENVIRONEMENT);
}

color	paint_mat_debug_unlit(Bool did_hit, hit_record *rec, ray *r)
{
	color	e;

	if (v.mat_debugmode == NORMAL)
	{
		if (!did_hit)
			return (BLACK);
		return (v3(-rec->normal.x, -rec->normal.z, rec->normal.y));
	}
	else if (v.mat_debugmode == MIST)
		return (paint_dist(did_hit, rec->t));
	else if (v.mat_debugmode == EMISSION)
	{
		if (!did_hit)
			return (BLACK);
		e = evaluate(&rec->mat.emission, rec->u, rec->v);
		e = v_scal(e, rec->mat.emission_strength);
		return (e);
	}
	else if (v.mat_debugmode == ENVIRONEMENT)
	{
		if (did_hit)
			return (BLACK);
		return (paint_env(r->dir));
	}
	return (ERROR_CYAN);
}

color	paint_mat_debug_lit(ray *ray_in, hit_record *rec)
{
	shader_end	se;

	se = pbr_lighting(rec, ray_in);
	if (v.mat_debugmode == DIFFUSE_LIGHT)
		return (se.diffuse_light);
	else if (v.mat_debugmode == DIFFUSE_COLOR)
		return (se.diffuse_color);
	else if (v.mat_debugmode == SPECULAR_LIGHT)
		return (se.specular_light);
	else if (v.mat_debugmode == SPECULAR_COLOR)
		return (se.specular_color);
	else if (v.mat_debugmode == COMBINED)
		return (paint_se(&se));
	return (ERROR_CYAN);
}
