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

// Linear fog
// PFPN f = (MAX_DIST - d) / (MAX_DIST);
// Exponential fog
// PFPN f = (1.0) / (pow(2.7, d * density));
// Exponential fog 2
//(used)
color	paint_dist(bool did_hit, PFPN d)
{
	PFPN	density;
	PFPN	f;

	if (d < 0 || !did_hit)
		d = v.far;
	density = 0.03;
	f = (1.0) / (pow(pow(2.7, d * density), 2));
	return (lerp(f, BLACK, WHITE));
}

color	paint_uv(bool did_hit, hit_record *rec)
{
	if (did_hit)
		return (evaluate(&v.uv_debug, rec->u, rec->v));
	return (BLACK);
}

color	paint_env(vec3 r_dir)
{
	vec3	uv;
	color	irradiance;

	uv = v_scal(v_add(r_dir, v_3(1)), 0.5);
	irradiance = evaluate(&v.irradiance_map, uv.x, uv.y);
	return (irradiance);
}

color	mat_values(bool did_hit, hit_record *rec)
{
	if (!did_hit)
		return (BLACK);
	if (v.mat_valuesmode == V_ALBEDO)
		return (evaluate(&rec->mat.base_color, rec->u, rec->v));
	else if (v.mat_valuesmode == V_METALIC)
		return (evaluate(&rec->mat.metalic, rec->u, rec->v));
	else if (v.mat_valuesmode == V_SPECULAR)
		return (evaluate(&rec->mat.specular, rec->u, rec->v));
	else if (v.mat_valuesmode == V_ROUGHNESS)
		return (evaluate(&rec->mat.roughness, rec->u, rec->v));
	else if (v.mat_valuesmode == V_TRANSMISSION)
		return (evaluate(&rec->mat.transmission, rec->u, rec->v));
	else if (v.mat_valuesmode == V_TRANSMISSION_ROUGHNESS)
		return (evaluate(&rec->mat.transmission_roughness, rec->u, rec->v));
	else if (v.mat_valuesmode == V_EMISSION)
		return (evaluate(&rec->mat.emission, rec->u, rec->v));
	else if (v.mat_valuesmode == V_NORMAL)
		return (evaluate(&rec->mat.normal, rec->u, rec->v));
	else if (v.mat_valuesmode == V_ALPHA)
		return (evaluate(&rec->mat.alpha, rec->u, rec->v));
	return (ERROR_CYAN);
}

color	paint_se(shader_end *se)
{
	return (v_add(v_mult(se->specular_color, se->specular_light),
			v_mult(se->diffuse_color, se->diffuse_light)));
}
