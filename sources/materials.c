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

	m.specular = 0;//0.5;
	m.specular_tint = 0.0;

	m.roughness = BW_MAP(1.0);

	m.ior = 1.45;
	m.transmission = 0.0;
	m.transmission_roughness = NO_MAP;

	m.emission = NO_MAP;
	m.emission_strength = 0.0;

	m.bump = NO_MAP;
	return m;
}

Bool	PBR_scatter(ray *ray_in, hit_record *rec, color *emitted_light, color *material_color, ray *scattered, material *self)
{
	vec3	scatter_dir;

	vec3	diffuse_dir = v_add(rec->normal, random_unit_vector());
	if (near_zero(diffuse_dir))
    {
		diffuse_dir = rec->normal;
	}
	Bool	isSpecularBounce = self->specular >= random_double();
	
	if (isSpecularBounce)
	{
		double roughnne = evaluate_bw(&(self->roughness), rec->u, rec->v);
		vec3	specular_dir = reflect(ray_in->dir, v_add(rec->normal,
					v_scal(random_unit_vector(), roughnne)));

		double	smoothness = 1.0 - roughnne;
		scatter_dir = lerp(smoothness * isSpecularBounce, diffuse_dir, specular_dir);
	}
	else
		scatter_dir = diffuse_dir;

	scatter_dir = v_norm(scatter_dir);
    *scattered = (ray){rec->p, scatter_dir};
	*emitted_light = evaluate(&(self->emission), rec->u, rec->v);
	*emitted_light = v_scal(*emitted_light, self->emission_strength);
	*material_color = lerp((1 - self->specular_tint) * isSpecularBounce, evaluate(&(self->base_color), rec->u, rec->v), WHITE);
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
	m.bump = bump;
    return m;
}

material new_metal(texture t, double roughness)
{
    material m = default_material();
	m.base_color = t;
	m.roughness = BW_MAP(roughness);
	NOT_IMPLEMENTED("textured roughness");
    return m;
}

material new_dielectric(texture t, double ior)
{
    material m = default_material();
	m.base_color = t;
	m.ior = ior;
	m.transmission = 1.0;
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

