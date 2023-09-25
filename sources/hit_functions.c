/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void    set_face_normal(hit_record *rec, const ray *r, vec3 outward_normal)
{
    // Sets the hit record normal vector.
    // NOTE: the parameter `outward_normal` is assumed to have unit length.

    rec->front_face = v_dot(r->dir, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal : v_scal(outward_normal, -1);
}

void    set_sphere_uv(vec3 p, hit_record *rec)
{
    double theta = acos(-p.y);
    double phi = atan2(-p.z, p.x) + MYPI;

    rec->u = phi / (2*MYPI);
    rec->v = theta / MYPI;
}

Bool	hit_sphere(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    vec3 center = self->pos;
    double radius = self->scale.x; //fix
    
    vec3 oc = v_sub(r->orig, center);
    double a = v_dot(r->dir, r->dir);
    double half_b = v_dot(oc, r->dir);
    double c = v_dot(oc, oc) - radius*radius;
    
    double discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return False;
    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (!surrounds(ray_t, root))
    {
        root = (-half_b + sqrtd) / a;
        if (!surrounds(ray_t, root))
            return False;
    }

    rec->t = root;
    rec->p = ray_at(r, rec->t);
    vec3 outward_normal = v_scal(v_sub(rec->p, center),  1.0 / radius);
    set_face_normal(rec, r, outward_normal);
    set_sphere_uv(outward_normal, rec);
    rec->mat = self->mat;

    return True;
}

void    set_plane_uv(double alpha, double beta, hit_record *rec)
{
    alpha-=(int)alpha/1;
    beta-=(int)beta/1;
    if (alpha<0)
        alpha = 1-fabs(alpha);
    if (beta<0)
        beta =  1-fabs(beta);
    rec->u = alpha;
    rec->v = beta;
}

Bool    hit_plane(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    vec3 Q = self->pos;
    vec3 u = rotate3(v3(self->scale.x,0,0), self->rot);
    vec3 v = rotate3(v3(0,0,self->scale.z), self->rot);
    vec3 n = v_cross(u, v);
    vec3 normal = v_norm(n);
    vec3 w = v_scal(n, 1.0/v_dot(n,n));
    //vec3 normal = v_norm(rotate3(v3(0,1,0), self->rot));
    
    double D = v_dot(normal, Q);

    double denom = v_dot(normal, r->dir);

    //No hit if parallel
    if (fabs(denom) < 0.001) return False;

    // Return false if the hit point parameter t is outside the ray interval.
    double t = (D - v_dot(normal, r->orig)) / denom;
    if (!contains(ray_t, t)) return False;

    vec3 intersection = ray_at(r, t);

    vec3 planar_hitpt_vector = v_sub(intersection, Q);
    double alpha = v_dot(w, v_cross(planar_hitpt_vector, v));
    double beta = v_dot(w, v_cross(u, planar_hitpt_vector));
    alpha = alpha/2 + .5;
    beta = 1 - (beta/2 + .5);
    
    rec->t = t;
    rec->p = intersection;
    rec->mat = self->mat;
    set_face_normal(rec, r, normal);
    set_plane_uv(alpha, beta, rec);

    return True;
}

Bool    is_interior(double a, double b, hit_record *rec)
{
    // Given the hit point in plane coordinates, return false if it is outside the
    // primitive, otherwise set the hit record UV coordinates and return true.

    if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
        return False;

    //rec.u = a;
    //rec.v = b;
    return True;
}

Bool    hit_quad(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    vec3 Q = self->pos;
    vec3 u = rotate3(v3(self->scale.x,0,0), self->rot);
    vec3 v = rotate3(v3(0,0,self->scale.z), self->rot);
    vec3 n = v_cross(u, v);
    vec3 normal = v_norm(n);
    vec3 w = v_scal(n, 1.0/v_dot(n,n));
    //vec3 normal = v_norm(rotate3(v3(0,1,0), self->rot));
    double D = v_dot(normal, Q);

    double denom = v_dot(normal, r->dir);

    //No hit if parallel
    if (fabs(denom) < 1e-8) return False;

    // Return false if the hit point parameter t is outside the ray interval.
    double t = (D - v_dot(normal, r->orig)) / denom;
    if (!contains(ray_t, t)) return False;

    // Determine the hit point lies within the planar shape using its plane coordinates.
    vec3 intersection = ray_at(r, t);
    vec3 planar_hitpt_vector = v_sub(intersection, Q);
    double alpha = v_dot(w, v_cross(planar_hitpt_vector, v));
    double beta = v_dot(w, v_cross(u, planar_hitpt_vector));

    alpha = alpha/2 + .5;
    beta = beta/2 + .5;

    if (!is_interior(alpha, beta, rec))
        return False;
    rec->u = alpha;
    rec->v = 1 - beta;

    // Ray hits the 2D shape; set the rest of the hit record and return true.
    rec->t = t;
    rec->p = intersection;
    rec->mat = self->mat;
    set_face_normal(rec, r, normal);

    return True;
}

Bool    hit_ss_quad(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    vec3 Q = self->pos;
    vec3 u = rotate3(v3(self->scale.x,0,0), self->rot);
    vec3 v = rotate3(v3(0,0,self->scale.z), self->rot);
    vec3 n = v_cross(u, v);
    vec3 normal = v_norm(n);
    vec3 w = v_scal(n, 1.0/v_dot(n,n));
    //vec3 normal = v_norm(rotate3(v3(0,1,0), self->rot));
    double D = v_dot(normal, Q);

    double denom = v_dot(normal, r->dir);

    //No hit if parallel
    if (fabs(denom) < 1e-8) return False;
    //No hit from the back
    if (denom < 0) return False;

    // Return false if the hit point parameter t is outside the ray interval.
    double t = (D - v_dot(normal, r->orig)) / denom;
    if (!contains(ray_t, t)) return False;

    // Determine the hit point lies within the planar shape using its plane coordinates.
    vec3 intersection = ray_at(r, t);
    vec3 planar_hitpt_vector = v_sub(intersection, Q);
    double alpha = v_dot(w, v_cross(planar_hitpt_vector, v));
    double beta = v_dot(w, v_cross(u, planar_hitpt_vector));

    alpha = alpha/2 + .5;
    beta = beta/2 + .5;

    if (!is_interior(alpha, beta, rec))
        return False;
    rec->u = alpha;
    rec->v = beta;

    // Ray hits the 2D shape; set the rest of the hit record and return true.
    rec->t = t;
    rec->p = intersection;
    rec->mat = self->mat;
    set_face_normal(rec, r, normal);

    return True;
}

//--NOT IMPLEMENTED YET

Bool    hit_box(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    return hit_sphere(r, ray_t, rec, self);
}

//FIX rotated cylinder
Bool    hit_cylinder(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    return hit_sphere(r, ray_t, rec, self);
}

Bool    hit_pyramid(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    return hit_sphere(r, ray_t, rec, self);
}

Bool    hit_line(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    return hit_sphere(r, ray_t, rec, self);
}

Bool	hit(const ray *r, const interval ray_t, hit_record *rec)
{
	hit_record	temp_rec;
	Bool		hit_anything = False;
	double		closest_so_far = ray_t.max;

	t_item item;
	for (int i=0; i<v.item_count; i++)
	{
		item = v.items[i];
		if (item.hit(r, (interval){ray_t.min, closest_so_far}, &temp_rec, &item))
		{
			hit_anything = True;
			closest_so_far = temp_rec.t;
		}
	}
    *rec = temp_rec;

	return hit_anything;
}
