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

void    set_face_normal(hit_record *rec, const ray *r, const vec3 outward_normal)
{
    // Sets the hit record normal vector.
    // NOTE: the parameter `outward_normal` is assumed to have unit length.

    rec->front_face = v_dot(r->dir, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal : v_scal(outward_normal, -1);
}

void    set_sphere_uv(hit_record *rec, const vec3 p, const vec3 sphere_rot)
{
    //p = rotate3(p, sphere_rot);

    double theta = acos(-p.y);
    double phi = atan2(-p.z, p.x) + MYPI;

    rec->u = phi / (2*MYPI);
    rec->v = theta / MYPI;

    rec->u = 1 - rec->u;

    //temporary fix to texture not rotating properly
    rec->v += ((int)(sphere_rot.x * RAD2DEG) % 360) /360.0;
}

//If you find a better quicker way to go from local_t to global
//tell me.
double  t2global(const double lt, const ray *local_r, const ray *r, const m4x4 fwd)
{
    vec3 temp;
    multiply_matrix_vector(fwd, ray_at(local_r, lt), &temp);
    return vec_dist(r->orig, temp);
}

//sphere normal based on local UV
Bool	hit_sphere(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    ray local_r = apply_ray(r, self->bck);

    //a = 1.0

    double b = 2.0 * v_dot(local_r.orig, local_r.dir);

    double c = v_dot(local_r.orig, local_r.orig) - 1.0;

    double discriminant = (b*b) - 4.0 * c;
    
    if (discriminant < 0) return False;
    double sqrtd = sqrt(discriminant);

    double t1 = (-b + sqrtd) / 2.0;
    double t2 = (-b - sqrtd) / 2.0;
    //If inside, ignore
    if ((t1 < 0.0) || (t2 < 0.0))
    {
        return False;
    }

    double lt, gt;
    // Determine which point of intersection was closest to the camera.
    lt = (t1 < t2)? t1 : t2;
    gt = t2global(lt, &local_r, r, self->fwd);

    if (!surrounds(ray_t, gt))
        return False;

    rec->t = gt;
    rec->p = ray_at(r, gt);
    set_face_normal(rec, &local_r, ray_at(&local_r, lt));
    set_sphere_uv(rec, rec->normal, self->rot);
    rec->mat = self->mat;
    
    return True;
}

void    set_plane_uv(double alpha, double beta, hit_record *rec)
{
    alpha-=(int)alpha/1;
    beta-=(int)beta/1;
    if (alpha<0)
        alpha = 1 - fabs(alpha);
    if (beta<0)
        beta = 1 - fabs(beta);
    rec->u = 1 - alpha;
    rec->v = beta;
}

Bool    hit_plane(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    ray local_r = apply_ray(r, self->bck);
    
    const vec3 n = v3(0,1,0);
    
    //No hit if parallel
    if (fabs(local_r.dir.y) < 1e-8) return False;

    // Return false if the hit point parameter t is outside the ray interval.
    double lt = local_r.orig.y / -local_r.dir.y;
    if (lt < 0.0) return False;

    double gt = t2global(lt, &local_r, r, self->fwd);
    if (!contains(ray_t, gt)) return False;

    vec3 localIntPoint = ray_at(&local_r, lt);
    vec3 intPoint = ray_at(r, gt);
    double alpha = localIntPoint.x;
    double beta = localIntPoint.z;
    alpha = .5 - alpha/2;
    beta  = .5 - beta/2;

    rec->t = gt;
    rec->p = intPoint;

    vec3 out_normal = rotate3(v3(0,1,0), self->rot);
    set_face_normal(rec, r, out_normal);


    rec->u = alpha;
    rec->v = beta;
    //set_plane_uv(alpha, beta, rec);
    
    rec->mat = self->mat;

    return True;
}

Bool    is_interior(const double a, const double b)
{
    if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
        return False;
    return True;
}

Bool    hit_quad(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    ray local_r = apply_ray(r, self->bck);
    
    //No hit if parallel
    if (fabs(local_r.dir.y) < 1e-8) return False;

    // Return false if the hit point parameter t is outside the ray interval.
    double lt = local_r.orig.y / -local_r.dir.y;
    if (lt < 0.0) return False;

    vec3 localIntPoint = ray_at(&local_r, lt);
    double alpha = localIntPoint.x;
    double beta = localIntPoint.z;
    alpha = .5 - alpha/2;
    beta  = .5 - beta/2;

    double gt = t2global(lt, &local_r, r, self->fwd);
    if (!contains(ray_t, gt)) return False;

    if (!is_interior(alpha, beta))
        return False;

    rec->u = alpha;
    rec->v = beta;

    rec->t = gt;
    rec->p = ray_at(r, gt);

    set_face_normal(rec, r, rotate3(v3(0,1,0), self->rot));

    rec->mat = self->mat;

    return True;
}

Bool    hit_ss_quad(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    ray local_r = apply_ray(r, self->bck);
    
    //No hit if parallel
    if (fabs(local_r.dir.y) < 1e-8) return False;
    //No hit from the back
    if (local_r.dir.y > 0) return False;

    // Return false if the hit point parameter t is outside the ray interval.
    double lt = local_r.orig.y / -local_r.dir.y;
    if (lt < 0.0) return False;

    vec3 localIntPoint = ray_at(&local_r, lt);
    double alpha = localIntPoint.x;
    double beta = localIntPoint.z;
    alpha = .5 - alpha/2;
    beta  = .5 - beta/2;
    
    if (!is_interior(alpha, beta))
        return False;

    double gt = t2global(lt, &local_r, r, self->fwd);
    if (!contains(ray_t, gt)) return False;

    rec->u = alpha;
    rec->v = beta;

    rec->t = gt;
    rec->p = ray_at(r, gt);

    set_face_normal(rec, r, rotate3(v3(0,1,0), self->rot));

    rec->mat = self->mat;

    return True;
}

vec3    get_cube_normal(const int id, hit_record *rec)
{
    return v3(1 * (id==3) - 1 * (id==2),
              1 * (id==5) - 1 * (id==4),
              1 * (id==0) - 1 * (id==1));
}

Bool    hit_box(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    ray local_r = apply_ray(r, self->bck);
    
    // Moved these here from the header file.
	double t[6]; //local t's
	double u[6];
	double v[6];
	
	// Extract values of a.
	double ax = local_r.orig.x;
	double ay = local_r.orig.y;
	double az = local_r.orig.z;
	
	double kx = local_r.dir.x;
	double ky = local_r.dir.y;
	double kz = local_r.dir.z;
		
	// Test for intersections with each plane (side of the box).
	// Top and bottom.
    //front back
	if (!close_enough(kz))
	{
		t[0] = (az - 1.0) / -kz;
		t[1] = (az + 1.0) / -kz;
		u[0] = ax + kx * t[0];
		v[0] = ay + ky * t[0];
		u[1] = ax + kx * t[1];
		v[1] = ay + ky * t[1];
	}
	else
	{
		t[0] = 100e6;
		t[1] = 100e6;
		u[0] = 0.0;
		v[0] = 0.0;
		u[1] = 0.0;
		v[1] = 0.0;
	}
	
	// Left and right.
	if (!close_enough(kx))
	{
		t[2] = (ax + 1.0) / -kx;
		t[3] = (ax - 1.0) / -kx;
		u[2] = az + kz * t[2];
		v[2] = ay + ky * t[2];
		u[3] = az + kz * t[3];
		v[3] = ay + ky * t[3];
	}
	else
	{
		t[2] = 100e6;
		t[3] = 100e6;
		u[2] = 0.0;
		v[2] = 0.0;
		u[3] = 0.0;
		v[3] = 0.0;		
	}
	
	// Front and back.
    //top bot
	if (!close_enough(ky))
	{
		t[4] = (ay + 1.0) / -ky;
		t[5] = (ay - 1.0) / -ky;
		u[4] = ax + kx * t[4];
		v[4] = az + kz * t[4];
		u[5] = ax + kx * t[5];
		v[5] = az + kz * t[5];
	}
	else
	{
		t[4] = 100e6;
		t[5] = 100e6;
		u[4] = 0.0;
		v[4] = 0.0;
		u[5] = 0.0;
		v[5] = 0.0;		
	}

    double finalT = 100e6;
    int final_index = 0;
    Bool valid_intersection = False;
    for (int i=0; i<6; i++)
    {
        if (t[i] < finalT && t[i] > 0.0)
        {
            double alpha = u[i];
            double beta = v[i];
            alpha = .5 - alpha/2;
            beta  = .5 - beta/2;
            
            if (is_interior(alpha, beta))
            {
                finalT = t[i];
                final_index = i;
                valid_intersection = True;
            }
        }
    }
    if (!valid_intersection)
        return False;

    double lt = t[final_index];
    double alpha = u[final_index];
    double beta  = v[final_index];
    alpha = .5 - alpha/2;
    beta  = .5 - beta/2;

    double gt = t2global(lt, &local_r, r, self->fwd);
    if (!contains(ray_t, gt))
        return False;

    rec->u = alpha;
    rec->v = beta;

    rec->t = gt;
    rec->p = ray_at(r, gt);

    vec3 outward_normal = get_cube_normal(final_index, rec);
    outward_normal = rotate3(outward_normal, self->rot);
    set_face_normal(rec, r, outward_normal);

    rec->mat = self->mat;

    return True;
}

Bool    hit_box_old(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    t_item side = (t_item){};
    side.mat = self->mat;

    double _w = self->scale.x;
    double _h = self->scale.y;
    double _d = self->scale.z;

    Bool got_hit = False;

    //Front
    side.pos = parent_to(v3(0,0,1), self);
    side.rot = v_add(v3(MYPI/2), self->rot);
    side.scale = v3(_w, 0, _h);
    got_hit |= hit_ss_quad(r, ray_t, rec, &side);
    //back
    side.pos = parent_to(v3(0,0,-1), self);
    side.rot = v_add(v3(-MYPI/2), self->rot);
    got_hit |= hit_ss_quad(r, ray_t, rec, &side);

    //Top
    side.pos = parent_to(v3(0,1,0), self);
    side.rot = self->rot;
    side.scale = v3(_w, 0, _d);
    got_hit |= hit_ss_quad(r, ray_t, rec, &side);
    //Bot
    side.pos = parent_to(v3(0,-1,0), self);
    side.rot = v_add(v3(MYPI), self->rot);
    got_hit |= hit_ss_quad(r, ray_t, rec, &side);

    //Right
    side.pos = parent_to(v3(-1,0,0), self);
    side.rot = v3(0,0,MYPI/2);
    side.scale = v3(_h, 0, _d);
    got_hit |= hit_ss_quad(r, ray_t, rec, &side);
    //Left
    side.pos = parent_to(v3(1,0,0), self);
    side.rot = v3(0,0,-MYPI/2);
    got_hit |= hit_ss_quad(r, ray_t, rec, &side);

    return got_hit;
}

//FIX rotated cylinder
Bool    hit_cylinder(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    ray local_r = apply_ray(r, self->bck);
    
    // Extract values of a.
	double ax = local_r.orig.x;
	double ay = local_r.orig.y;
	double az = local_r.orig.z;
	
	double kx = local_r.dir.x;
	double ky = local_r.dir.y;
	double kz = local_r.dir.z;

    // Compute a, b and c.
    double a = (kx * kx) + (kz * kz);
    double b = 2.0 * (ax * kx + az * kz);
    double c = ((ax * ax) + (az * az)) - 1.0;

	// Compute b^2 - 4ac.
	double root = sqrt((b*b) - 4.0 * a * c);

	double t[4]; //local t's
    double u[4];
    double v[4];
    
    if (root > 0.0)
    {
		t[0] = (-b + root) / (2 * a);
		t[1] = (-b - root) / (2 * a);

        if (fabs(ay + ky * t[0]) > 1.0)
            t[0] = 100e6;
        if (fabs(ay + ky * t[1]) > 1.0)
            t[1] = 100e6;
    }
    else
    {
        t[0] = 100e6;
        t[1] = 100e6;
    }
    
    // Top and bottom CAPS
	if (!close_enough(ky))
	{
		t[2] = (ay - 1.0) / -ky;
		t[3] = (ay + 1.0) / -ky;
        u[2] = ax + kx * t[2];
		v[2] = az + kz * t[2];
        if (u[2]*u[2] + v[2]*v[2] > 1.0)
            t[2] = 100e6;
        u[3] = ax + kx * t[3];
		v[3] = az + kz * t[3];
        if (u[3]*u[3] + v[3]*v[3] > 1.0)
            t[3] = 100e6;
	}
	else
	{
		t[2] = 100e6;
		t[3] = 100e6;
	}

    double finalT = 100e6;
    int final_index = 0;
    Bool valid_intersection = False;
    for (int i=0; i<4; i++)
    {
        if (t[i] < finalT && t[i] > 0.0)
        {
            finalT = t[i];
            final_index = i;
            valid_intersection = True;
        }
    }
    if (!valid_intersection)
        return False;

//----confirmed hit
    double lt = t[final_index];
    double lu = u[final_index];
    double lv = v[final_index];

    vec3 local_p = ray_at(&local_r, lt);

    double gt = t2global(lt, &local_r, r, self->fwd);
    if (!contains(ray_t, gt))
        return False;

    rec->t = gt;
    rec->p = ray_at(r, gt);


    vec3 outward_normal;
    if (final_index <= 1) //cylinder hit
    {
        outward_normal = v3(local_p.x, 0, local_p.z);
        outward_normal = rotate3(outward_normal, self->rot);
        rec->u = (atan2(local_p.z, local_p.x) + MYPI)/(2*MYPI);
        rec->v = local_p.y/2 + .5;
    }
    else //caps
    {
        outward_normal = v3(0, 1 * (final_index==2) - 1 * (final_index==3),0);
        outward_normal = rotate3(outward_normal, self->rot);
        rec->u = .5 - lu/2;
        rec->v = .5 - lv/2;
    }
    set_face_normal(rec, r, outward_normal);

    
    //UV
    
    rec->mat = self->mat;

    return True;
}

Bool    hit_cone(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    ray local_r = apply_ray(r, self->bck);
    
    const double h_offset = 1.0;
    const double r_factor = 2.0;

    // Extract values of a.
	double ax = local_r.orig.x * r_factor;
	double ay = local_r.orig.y - h_offset;
	double az = local_r.orig.z * r_factor;
	
	double kx = local_r.dir.x * r_factor;
	double ky = local_r.dir.y;
	double kz = local_r.dir.z * r_factor;

	double a = (kx * kx) + (kz * kz) - (ky * ky);
	double b = 2.0 * (ax * kx + az * kz - ay * ky);
	double c = (ax * ax) + (az * az) - (ay * ay);

	// Compute b^2 - 4ac.
	double root = sqrt((b*b) - 4.0 * a * c);

	double t[3]; //local t's
    
    if (root > 0.0)
    {
		t[0] = (-b + root) / (2 * a);
		t[1] = (-b - root) / (2 * a);

        double oi0 = ay + ky * t[0];
        double oi1 = ay + ky * t[1];

        if (oi0 > 0.0 || oi0 < -1.0 * r_factor)
            t[0] = 100e6;
        if (oi1 > 0.0|| oi1 < -1.0 * r_factor)
            t[1] = 100e6;
    }
    else
    {
        t[0] = 100e6;
        t[1] = 100e6;
    }
    
    // Top and bottom CAPS
	if (!close_enough(ky))
	{
		t[2] = (ay + 1.0 * r_factor) / -ky;
        double u = ax + kx * t[2];
		double v = az + kz * t[2];
        if (u*u + v*v > 1.0 * (r_factor * r_factor))
            t[2] = 100e6;
	}
	else
	{
		t[2] = 100e6;
	}

    double finalT = 100e6;
    int final_index = 0;
    Bool valid_intersection = False;
    for (int i=0; i<3; i++)
    {
        if (t[i] < finalT && t[i] > 0.0)
        {
            finalT = t[i];
            final_index = i;
            valid_intersection = True;
        }
    }
    if (!valid_intersection)
        return False;

//----confirmed hit
    double lt = t[final_index];

    vec3 local_p = ray_at(&local_r, lt);

    double gt = t2global(lt, &local_r, r, self->fwd);
    if (!contains(ray_t, gt))
        return False;

    rec->t = gt;
    rec->p = ray_at(r, gt);

    vec3 outward_normal;
    if (final_index <= 1) //cone hit
    {
        outward_normal.x = local_p.x;
        outward_normal.z = local_p.z;
		outward_normal.y = sqrtf((local_p.x*local_p.x) + (local_p.z*local_p.z)) / r_factor;
        rec->u = (atan2(local_p.z, local_p.x) + MYPI)/(2*MYPI);
        rec->v = local_p.y/2 + .5;
    }
    else //bot
    {
        outward_normal = v3(0, -1, 0);
        rec->u = .5 - local_p.x/2;
        rec->v = .5 - local_p.z/2;
    }
    // outward_normal = rotate3(outward_normal, self->rot);
    rec->normal = outward_normal;
    //set_face_normal(rec, r, outward_normal);

    //UV
    
    rec->mat = self->mat;

    return True;
}

//--NOT IMPLEMENTED YET

// -----------------------------------------------------------

Bool    hit_line(const ray *r, const interval ray_t, hit_record *rec, const t_item *self)
{
    return hit_sphere(r, ray_t, rec, self);
}

Bool	hit(const ray *r, const interval ray_t, hit_record *rec)
{
	hit_record	temp_rec;
	Bool		hit_anything = False;
	double		closest_so_far = ray_t.max;

	for (int i=0; i<v.item_count; i++)
	{
		if (v.items[i].hit(r, (interval){ray_t.min, closest_so_far}, &temp_rec, &v.items[i]))
		{
			hit_anything = True;
			closest_so_far = temp_rec.t;
		}
	}
    *rec = temp_rec;

	return hit_anything;
}

Bool	check_hit(const ray *r, const interval ray_t)
{
	hit_record	temp_rec;

	for (int i=0; i<v.item_count; i++)
	{
		if (v.items[i].hit(r, ray_t, &temp_rec, &v.items[i]))
		{
			return True;
		}
	}

	return False;
}
