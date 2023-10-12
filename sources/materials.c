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

Bool    lambertian_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self);
Bool    metal_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self);
Bool    dielectric_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self);
Bool    no_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self);

color   no_emission(double u, double v, vec3 *p);
color   diffuse_light(double u, double v, vec3 *p);

material	default_material(void)
{
	material m;
	m.base_color = WHITE_MAP;
	m.metalic = NO_MAP;

	m.specular = NO_MAP;
	m.specular_tint = 0.0;

	m.roughness = NO_MAP;

	m.ior = 1.0;
	m.transmission = 0.0;
	m.transmission_roughness = NO_MAP;

	m.emission = NO_MAP;
	m.emission_strength = 0.0;

	m.normal = c3(0.5, 0.5, 1.0);
	m.normal_strength = 1.0;
	return m;
}

Bool	PBR_scatter(const ray *ray_in, hit_record *rec, ray *scattered, Bool *was_specular)
{
    *scattered = (ray){rec->p, rec->normal};
	return 0;
	
	material *self = &rec->mat;
	vec3	base = evaluate(&(self->base_color), rec->u, rec->v);
	double	metalic = evaluate_bw(&(self->metalic), rec->u, rec->v);
	double	specular = evaluate_bw(&(self->specular), rec->u, rec->v);
	double	roughness = evaluate_bw(&(self->roughness), rec->u, rec->v);
	double	t_roughness = evaluate_bw(&(self->transmission_roughness), rec->u, rec->v);

	//? contine

	vec3	scatter_dir;

	vec3	diffuse_dir = v_add(rec->normal, random_unit_vector());

	if (near_zero(diffuse_dir))
    {
		diffuse_dir = rec->normal;
	}
	Bool	isSpecularBounce = specular >= random_double() || metalic;
	
	if (isSpecularBounce)
	{
		vec3	reflection_dir = reflect_safe(ray_in->dir, v_add(rec->normal,
					v_scal(random_unit_vector(), roughness)), rec->old_normal);

		double	smoothness = 1.0 - roughness;
		scatter_dir = lerp(smoothness, diffuse_dir, reflection_dir);
	}
	else if (self->transmission > random_double())
	{
		double refraction_ratio = rec->front_face ? (1.0/self->ior) : self->ior;

		double cos_theta = fmin(v_dot(v_scal(ray_in->dir, -1.0), rec->normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

		Bool cannot_refract = refraction_ratio * sin_theta > 1.0;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
		{
			scatter_dir = reflect(ray_in->dir, v_add(rec->normal,
					v_scal(random_unit_vector(), roughness)));
			isSpecularBounce = True;
		}
		else
			scatter_dir = refract(ray_in->dir, v_add(rec->normal,
					v_scal(random_unit_vector(), t_roughness)), refraction_ratio);
	}
	else
		scatter_dir = diffuse_dir;

	scatter_dir = v_norm(scatter_dir);
    *scattered = (ray){rec->p, scatter_dir};
	*was_specular = isSpecularBounce;
    return True;
    (void) ray_in;
}

material new_lambertian(texture t)
{
	material m = default_material();
	m.base_color = t;
    return m;
}

material new_lambertian_bump(texture t, texture bump)
{
    material m = default_material();
	m.base_color = t;
	m.normal = bump;
    return m;
}

material new_metal(texture t, double roughness)
{
    material m = default_material();
	m.base_color = t;
	m.metalic = BW_MAP(1.0);
	m.roughness = BW_MAP(roughness);
	NOT_IMPLEMENTED("textured roughness");
    return m;
}

material new_dielectric(texture t, double ior)
{
    material m = default_material();
	m.base_color = t;
	m.ior = ior;
	m.transmission = 0.8;
	NOT_IMPLEMENTED("dielectric custom transmission values");
	m.transmission_roughness = NO_MAP;
	NOT_IMPLEMENTED("transmission roughness map");
    return m;
}

material new_light(texture t, double emission_strength)
{
	material m = default_material();
	m.base_color = NO_MAP;
	m.emission = t;
	m.emission_strength = emission_strength;
	return m;
}

//  ----- Scatters

Bool    no_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self)
{
    return False;
}

Bool    lambertian_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self)
{
    vec3    diffuse_dir = v_add(rec->normal, random_unit_vector());

    if (near_zero(diffuse_dir))
        diffuse_dir = rec->normal;

    *scattered = (ray){rec->p, diffuse_dir};
    *attenuation = self->base_color.value(rec->u, rec->v, &self->base_color);
    return True;
    (void) ray_in;
}

Bool    metal_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self)
{
    vec3    reflected = reflect(v_norm(ray_in->dir), rec->normal);

    *scattered = (ray){rec->p, v_add(reflected, v_scal(random_unit_vector(), 0.5))};//self->roughness.value(0,0, &self->roughness)))};
    *attenuation = self->base_color.value(rec->u, rec->v, &self->base_color);
    return v_dot(scattered->dir, rec->normal) > 0;
}

Bool    dielectric_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self)
{
	*attenuation = self->base_color.value(rec->u, rec->v, &self->base_color);
	double refraction_ratio = rec->front_face ? (1.0/self->ior) : self->ior;

	vec3 unit_direction = v_norm(ray_in->dir);
	double cos_theta = fmin(v_dot(v_scal(unit_direction, -1.0), rec->normal), 1.0);
	double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

	Bool cannot_refract = refraction_ratio * sin_theta > 1.0;
	vec3 direction;

	if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
		direction = reflect(unit_direction, rec->normal);
	else
		direction = refract(unit_direction, rec->normal, refraction_ratio);

	*scattered = (ray){rec->p, direction};
	return True;
}

