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

	m.specular = NO_MAP;//0.5;
	m.specular_tint = 0.0;

	m.roughness = BW_MAP(1.0);

	m.ior = 1.45;
	m.transmission = 0.0;
	m.transmission_roughness = NO_MAP;

	m.emission = NO_MAP;
	m.emission_strength = 0.0;

	m.normal = NO_MAP;
	return m;
}

/*
mat4 cotangent_frame( vec3 N, vec3 p, vec3 uv )
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec3 duv1 = dFdx( uv );
    vec3 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = v_cross( dp2, N );
    vec3 dp1perp = v_cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = 1.0 / sqrt( max( v_dot(T,T), v_dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal( vec3 N, vec3 V, vec3 texcoord , texture *mapBump )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
    vec3 map = evaluate( mapBump, texcoord.x, texcoord.y );
	mat4 TBN = cotangent_frame( N, v_scal(V, -1), texcoord );
    return v_norm(mult_point_matrix(map, TBN));
}
*/

Bool	PBR_scatter(ray *ray_in, hit_record *rec, color *emitted_light, color *material_color, ray *scattered, material *self)
{
	//BUMP map
	//vec3 new_normal = perturb_normal(rec->normal, v.camera_pos, v3(rec->u, rec->v), &(self->normal));
	vec3	normalRGB = evaluate(&(self->normal), rec->u, rec->v);
	normalRGB = v_scal(normalRGB, 100);
	//rec->normal = v_norm(v_mult(rec->normal, normalRGB));


	vec3	base = evaluate(&(self->base_color), rec->u, rec->v);
	double	metalic = evaluate_bw(&(self->metalic), rec->u, rec->v);
	double	specular = evaluate_bw(&(self->specular), rec->u, rec->v);
	double	roughness = evaluate_bw(&(self->roughness), rec->u, rec->v);
	double	t_roughness = evaluate_bw(&(self->transmission_roughness), rec->u, rec->v);
	vec3	emission = evaluate(&(self->emission), rec->u, rec->v);

	//? contine

	vec3	scatter_dir;

	vec3	diffuse_dir = v_add(rec->normal, random_unit_vector());

	if (near_zero(diffuse_dir))
    {
		diffuse_dir = rec->normal;
	}
	Bool	isSpecularBounce = specular >= random_double();
	
	if (isSpecularBounce || metalic)
	{
		vec3	reflection_dir = reflect(ray_in->dir, v_add(rec->normal,
					v_scal(random_unit_vector(), roughness)));

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
		}
		else
			scatter_dir = refract(ray_in->dir, v_add(rec->normal,
					v_scal(random_unit_vector(), t_roughness)), refraction_ratio);
	}
	else
		scatter_dir = diffuse_dir;

	scatter_dir = v_norm(scatter_dir);
    *scattered = (ray){rec->p, scatter_dir};
	*emitted_light = evaluate(&(self->emission), rec->u, rec->v);
	*emitted_light = v_scal(*emitted_light, self->emission_strength);
	if (isSpecularBounce)
		*material_color = WHITE; // lerp between white and base on tint
	else
		*material_color = base;
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

