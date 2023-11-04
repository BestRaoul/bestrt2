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

material	default_material(void)
{
	material	m;

	m.base_color = WHITE_MAP;
	m.metalic = NO_MAP;
	m.specular = NO_MAP;
	m.specular_tint = 0.0;
	m.roughness = NO_MAP;
	m.ior = 1.0;
	m.transmission = NO_MAP;
	m.transmission_roughness = NO_MAP;
	m.emission = NO_MAP;
	m.emission_strength = 0.0;
	m.normal = c3(0.5, 0.5, 1.0);
	m.normal_strength = v3(1, 1, 1);
	m.normal_mode = OPENGL;
	m.alpha = FULL_MAP;
	return (m);
}

material	new_m(vec3 color)
{
	return (new_lambertian(solid_color(color)));
}

material	new_m_rgb(vec3 rgbs)
{
	return (new_lambertian(solid_color(v_scal(rgbs, 1.0 / 256.0))));
}

//  ----- Scatters
/*
Bool		lambertian_scatter(ray *ray_in, hit_record *rec, color *attenuation,
				ray *scattered, material *self);
Bool		metal_scatter(ray *ray_in, hit_record *rec, color *attenuation,
				ray *scattered, material *self);
Bool		dielectric_scatter(ray *ray_in, hit_record *rec, color *attenuation,
				ray *scattered, material *self);
Bool		no_scatter(ray *ray_in, hit_record *rec, color *attenuation,
				ray *scattered, material *self);

Bool	no_scatter(ray *ray_in, hit_record *rec, color *attenuation,
ray *scattered, material *self)
{
	return (False);
}

Bool	lambertian_scatter(ray *ray_in, hit_record *rec, color *attenuation,
ray *scattered, material *self)
{
	vec3	diffuse_dir;

	diffuse_dir = v_add(rec->normal, random_unit_vector());
	if (near_zero(diffuse_dir))
		diffuse_dir = rec->normal;
	*scattered = (ray){rec->p, diffuse_dir};
	*attenuation = self->base_color.value(rec->u, rec->v, &self->base_color);
	return (True);
	(void) ray_in;
}

Bool	metal_scatter(ray *ray_in, hit_record *rec, color *attenuation,
ray *scattered, material *self)
{
	vec3	reflected;

	reflected = reflect(v_norm(ray_in->dir), rec->normal);
	*scattered = (ray){rec->p, v_add(reflected, v_scal(random_unit_vector(),
	0.5))};//self->roughness.value(0,0, &self->roughness)))};
	*attenuation = self->base_color.value(rec->u, rec->v, &self->base_color);
	return (v_dot(scattered->dir, rec->normal) > 0);
}

Bool	dielectric_scatter(ray *ray_in, hit_record *rec, color *attenuation,
ray *scattered, material *self)
{
	double	refraction_ratio;
	vec3	unit_direction;
	double	sin_theta;
	Bool	cannot_refract;
	vec3	direction;

	*attenuation = self->base_color.value(rec->u, rec->v, &self->base_color);
	refraction_ratio = rec->front_face ? (1.0/self->ior) : self->ior;
	unit_direction = v_norm(ray_in->dir);
	double cos_theta = fmin(v_dot(v_scal(unit_direction, -1.0), rec->normal),
	1.0);
	sin_theta = sqrt(1.0 - cos_theta*cos_theta);
	cannot_refract = refraction_ratio * sin_theta > 1.0;
	if (cannot_refract || reflectance(cos_theta, refraction_ratio)
	> random_double())
		direction = reflect(unit_direction, rec->normal);
	else
		direction = refract(unit_direction, rec->normal, refraction_ratio);
	*scattered = (ray){rec->p, direction};
	return (True);
}
*/
