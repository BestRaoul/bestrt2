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

material new_lambertian(texture t)
{
    material m;
    m.albedo = t;
    m.emissive = NO_MAP;
    m.scatter = lambertian_scatter;
    return m;
}

material new_metal(texture t, double fuzz)
{
    material m;
    m.albedo = t;
    m.emissive = NO_MAP;
    m.scatter = metal_scatter;
    m.fuzz = fuzz;
    return m;
}

material new_dielectric(texture t, double refraction)
{
    material m;
    m.albedo = t;
    m.emissive = NO_MAP;
    m.scatter = dielectric_scatter;
    m.refraction = refraction;
    return m;
}

material new_light(texture t)
{
    material m;
    m.emissive = t;
    m.scatter = no_scatter;
    return m;
}

//  ----- Scatters

Bool    no_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self)
{
    return False;
}

Bool    lambertian_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self)
{
    vec3    scatter_direction = v_add(rec->normal, random_unit_vector());

    if (near_zero(scatter_direction))
        scatter_direction = rec->normal;

    *scattered = (ray){rec->p, scatter_direction};
    *attenuation = self->albedo.value(rec->u, rec->v, &self->albedo);
    return True;
    (void) ray_in;
}

Bool    metal_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self)
{
    vec3    reflected = reflect(v_norm(ray_in->dir), rec->normal);

    *scattered = (ray){rec->p, v_add(reflected, v_scal(random_unit_vector(), self->fuzz))};
    *attenuation = self->albedo.value(rec->u, rec->v, &self->albedo);
    return v_dot(scattered->dir, rec->normal) > 0;
}

Bool    dielectric_scatter(ray *ray_in, hit_record *rec, color *attenuation, ray *scattered, material *self)
{
	*attenuation = self->albedo.value(rec->u, rec->v, &self->albedo);
	double refraction_ratio = rec->front_face ? (1.0/self->refraction) : self->refraction;

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

