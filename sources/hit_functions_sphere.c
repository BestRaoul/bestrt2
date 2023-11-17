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

static inline void	set_sphere_values(hit_record *rec,
	const ray *local_r, PFPN lt, const t_item *self)
{
	vec3	local_p;

	local_p = ray_at(local_r, lt);
	rec->u = (atan2(local_p.z, local_p.x) + MYPI) / (2 * MYPI);
	rec->v = local_p.y / 2 + .5;
	rec->mat = self->mat;
	set_face_normal(rec, local_r, local_p);
	maybe_apply_perturb(rec);
	rec->normal = rotate3(rec->normal, self->transform.rot);
	rec->old_normal = rotate3(rec->old_normal, self->transform.rot);
}

// a = 1.0
static inline bool	get_sphere_intersection(const ray *local_r,
	const interval ray_t, PFPN *t1, PFPN *t2)
{
	PFPN	b;
	PFPN	c;
	PFPN	discriminant;
	PFPN	sqrtd;

	b = 2.0 * v_dot(local_r->orig, local_r->dir);
	c = v_dot(local_r->orig, local_r->orig) - 1.0;
	discriminant = (b * b) - 4.0 * c;
	if (discriminant < 0)
		return (False);
	sqrtd = sqrt(discriminant);
	*t1 = (-b + sqrtd) / 2.0;
	*t2 = (-b - sqrtd) / 2.0;
	if (*t1 < 0.0)
		*t1 = ray_t.max;
	if (*t2 < 0.0)
		*t2 = ray_t.max;
	return (True);
}

// sphere normal based on local UV
bool	hit_sphere(const ray *r, const interval ray_t, hit_record *rec,
		const t_item *self)
{
	ray		local_r;
	PFPN	t1;
	PFPN	t2;
	PFPN	lt;
	PFPN	gt;

	local_r = apply_ray(r, self->bck);
	if (!get_sphere_intersection(&local_r, ray_t, &t1, &t2))
		return (False);
	if (t1 < t2)
		lt = t1;
	else
		lt = t2;
	gt = t2global(lt, &local_r, r, self->fwd);
	if (!surrounds(ray_t, gt))
		return (False);
	rec->t = gt;
	rec->p = ray_at(r, gt);
	set_sphere_values(rec, &local_r, lt, self);
	return (True);
}
