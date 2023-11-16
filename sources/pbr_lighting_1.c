/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

typedef struct s_pbr_data
{
	color		albedo;
	PFPN		metalic;
	PFPN		roughness;
	PFPN		specularity;
	PFPN		transmission;
	PFPN		r0;
	vec3		n;
	vec3		v;
	PFPN		n_dot_v;
	vec3		p;
}	t_pbr_data;

typedef struct s_bdrf_info
{
	t_pbr_data	*d;
	vec3		lit;
	vec3		half;
	PFPN		n_dot_l;
	PFPN		h_dot_v;
	PFPN		n_dot_h;
}	t_bdrf_info;

// se->specular_light = BLACK;
static void	set_ibl(shader_end *se, hit_record *rec, ray *ray_in, t_pbr_data *d)
{
	t_light	ibl;
	color	tint;
	vec3	uv;
	vec3	f;
	vec3	l;

	se->diffuse_color = v_scal(d->albedo, (1.0 - d->metalic));
	f = fresnel_shlick(d->n_dot_v, d->r0);
	tint = WHITE;
	if (v_len(d->albedo) > 0)
		tint = lerp(rec->mat.specular_tint, WHITE, v_norm(d->albedo));
	tint = lerp(f.x, tint, WHITE);
	se->specular_color = v_add(v_mult(f, tint), v_scal(d->albedo, d->metalic));
	if (v.use_ibl == False)
		return ;
	l = reflect_safe(ray_in->dir, v_add(rec->normal,
				v_scal(random_unit_vector(), d->roughness)), rec->old_normal);
	ibl = (t_light){(tfm){}, BLACK, v_scal(l, -1), 0, True};
	uv = v_scal(v_add(l, v_3(1)), 0.5);
	if (!(check_obstruction(&ibl, rec->p) || (d->metalic == 0.0
				&& d->specularity == 0.0 && d->transmission == 0.0)))
		se->specular_light = evaluate(&v.irradiance_map, uv.x, uv.y);
	se->diffuse_light = v_scal(v_mult(v_sub(v_3(1.0), f),
				evaluate(&v.blurry_irradiance_map, uv.x, uv.y)), (1.0
				- d->transmission));
}

static vec3	get_radiance(t_light *l, t_bdrf_info *info)
{
	PFPN	distance;
	PFPN	attenuation;

	if (l->is_dir)
		info->lit = v_scal(l->dir, -1);
	else
		info->lit = v_norm(from_to(info->d->p, l->transform.pos));
	info->half = v_norm(v_add(info->d->v, info->lit));
	distance = v_len(v_sub(l->transform.pos, info->d->p));
	if (l->is_dir)
		attenuation = 1.0;
	else
		attenuation = 1.0 / (1.0 + distance * distance);
	return (v_scal(l->col, attenuation * l->intensity));
}

// Cook-Torrence BRDF
static void	cook_torrence_brdf(t_bdrf_info *info, vec3 *specular, vec3 *kd)
{
	PFPN	h_dot_v;
	PFPN	n_dot_h;
	PFPN	d;
	PFPN	g;
	vec3	f;

	info->n_dot_l = fmax(v_dot(info->d->n, info->lit), 0.0000001);
	h_dot_v = fmax(v_dot(info->half, info->d->v), 0.0);
	n_dot_h = fmax(v_dot(info->d->n, info->half), 0.0);
	d = distribution_ggx(n_dot_h, info->d->roughness);
	g = geometry_smith(info->d->n_dot_v, info->n_dot_l, info->d->roughness);
	f = fresnel_shlick(h_dot_v, info->d->r0);
	*specular = v_scal(f, g * d);
	*specular = v_scal(*specular, 1. / (4. * info->d->n_dot_v * info->n_dot_l));
	*kd = v_sub(v_3(1.0), f);
}

static void	add_single_pbr_light(t_light *l, t_pbr_data *d, shader_end *se)
{
	t_bdrf_info	info;
	vec3		radiance;
	vec3		specular;
	vec3		kd;

	if (check_obstruction(l, d->p))
		return ;
	info.d = d;
	radiance = get_radiance(l, &info);
	cook_torrence_brdf(&info, &specular, &kd);
	radiance = v_scal(radiance, info.n_dot_l);
	se->diffuse_light = v_add(se->diffuse_light, v_mult(kd, v_scal(radiance,
					(1.0 - d->transmission))));
	se->specular_color = v_add(se->specular_color, specular);
	se->specular_light = v_add(se->specular_light, radiance);
}

shader_end	pbr_lighting(hit_record *rec, ray *ray_in)
{
	shader_end	se;
	t_pbr_data	d;
	int			i;

	se = (shader_end){};
	d.albedo = evaluate(&rec->mat.base_color, rec->u, rec->v);
	d.metalic = evaluate_bw(&rec->mat.metalic, rec->u, rec->v);
	d.roughness = evaluate_bw(&rec->mat.roughness, rec->u, rec->v);
	d.specularity = evaluate_bw(&rec->mat.specular, rec->u, rec->v);
	d.transmission = evaluate_bw(&rec->mat.transmission, rec->u, rec->v);
	d.r0 = d.specularity * 0.16 + (d.transmission) * 0.16;
	d.n = rec->normal;
	d.v = v_scal(ray_in->dir, -1);
	d.n_dot_v = fmax(v_dot(d.n, d.v), 0.0000001);
	d.p = rec->p;
	set_ibl(&se, rec, ray_in, &d);
	se.diffuse_light = v_add(se.diffuse_light, v_3(v.ambient));
	i = 0;
	while (i < v.light_count)
	{
		add_single_pbr_light(&(v.lights[i]), &d, &se);
		i++;
	}
	return (se);
}

// static void	extract_data(t_pbr_data *data, hit_record *rec, ray *ray_in)
// {
// 	data->albedo = evaluate(&rec->mat.base_color, rec->u, rec->v);
// 	data->metalic = evaluate_bw(&rec->mat.metalic, rec->u, rec->v);
// 	data->roughness = evaluate_bw(&rec->mat.roughness, rec->u, rec->v);
// 	data->specularity = evaluate_bw(&rec->mat.specular, rec->u, rec->v);
// 	data->transmission = evaluate_bw(&rec->mat.transmission, rec->u, rec->v);
// 	data->r0 = data->specularity * 0.16 + (data->transmission) * 0.16;
// 	data->n = rec->normal;
// 	data->v = v_scal(ray_in->dir, -1);
// 	data->n_dot_v = fmax(v_dot(data->n, data->v), 0.0000001);
// 	data->p = rec->p;
// }
