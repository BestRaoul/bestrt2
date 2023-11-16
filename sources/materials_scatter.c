/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   materials.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	specular_bounce(const ray *ray_in, hit_record *rec, vec3 *sd,
				vec3 diffuse_dir);
static void	reflecto_bounce(const ray *ray_in, hit_record *rec, vec3 *sd,
				Bool *is_specular_bounce);

void	pbr_scatter(const ray *ray_in, hit_record *rec, ray *scattered,
		Bool *was_specular)
{
	material	*self;
	PFPN		specular;
	vec3		scatter_dir;
	vec3		diffuse_dir;
	Bool		is_specular_bounce;

	self = &rec->mat;
	specular = evaluate_bw(&(self->specular), rec->u, rec->v);
	diffuse_dir = v_add(rec->normal, random_unit_vector());
	if (near_zero(diffuse_dir))
		diffuse_dir = rec->normal;
	is_specular_bounce = ((specular >= random_PFPN())
			|| evaluate_bw(&(self->metalic), rec->u, rec->v)
			>= random_PFPN());
	if (is_specular_bounce)
		specular_bounce(ray_in, rec, &scatter_dir, diffuse_dir);
	else if (evaluate_bw(&(self->transmission), rec->u, rec->v)
		> random_PFPN())
		reflecto_bounce(ray_in, rec, &scatter_dir, &is_specular_bounce);
	else
		scatter_dir = diffuse_dir;
	*scattered = (ray){rec->p, v_norm(scatter_dir)};
	*was_specular = is_specular_bounce;
}

static void	specular_bounce(const ray *ray_in, hit_record *rec,
		vec3 *scatter_dir, vec3 diffuse_dir)
{
	vec3		reflection_dir;
	PFPN		roughness;
	PFPN		smoothness;

	roughness = evaluate_bw(&(rec->mat.roughness), rec->u, rec->v);
	reflection_dir = reflect_safe(ray_in->dir, v_add(rec->normal,
				v_scal(random_unit_vector(), roughness)), rec->old_normal);
	smoothness = 1.0 - roughness;
	*scatter_dir = lerp(smoothness, diffuse_dir, reflection_dir);
}

static void	reflecto_bounce(const ray *ray_in, hit_record *rec,
	vec3 *scatter_dir, Bool *is_specular_bounce)
{
	PFPN		refraction_ratio;
	PFPN		cos_theta;
	PFPN		sin_theta;
	PFPN		t_roughness;
	PFPN		roughness;

	roughness = evaluate_bw(&(rec->mat.roughness), rec->u, rec->v);
	t_roughness = evaluate_bw(&(rec->mat.transmission_roughness),
			rec->u, rec->v);
	refraction_ratio = rec->front_face * (1.0 / rec->mat.ior)
		+ !rec->front_face * rec->mat.ior;
	cos_theta = fmin(v_dot(v_scal(ray_in->dir, -1.0), rec->normal), 1.0);
	sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	if (refraction_ratio * sin_theta > 1.0 || reflectance(cos_theta,
			refraction_ratio) > random_PFPN())
	{
		*scatter_dir = reflect_safe(ray_in->dir, v_add(rec->normal,
					v_scal(random_unit_vector(), roughness)),
				rec->old_normal);
		*is_specular_bounce = True;
	}
	else
		*scatter_dir = refract(ray_in->dir, v_add(rec->normal,
					v_scal(random_unit_vector(), t_roughness)),
				refraction_ratio);
}
