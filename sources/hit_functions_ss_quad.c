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

typedef struct s_qh
{
	PFPN			t;
	PFPN			u;
	PFPN			v;
	Bool			valid_intersection;
}					t_qh;

static inline void	get_ss_quad_intersection(const ray *local_r, t_qh *data)
{
	PFPN	lt;
	vec3	local_p;
	PFPN	alpha;
	PFPN	beta;

	if (local_r->dir.y > -1e-8)
		return ((void)(data->valid_intersection = False));
	lt = local_r->orig.y / -local_r->dir.y;
	if (lt < 0.0)
		return ((void)(data->valid_intersection = False));
	local_p = ray_at(local_r, lt);
	alpha = local_p.x;
	beta = local_p.z;
	alpha = .5 - alpha / 2;
	beta = .5 - beta / 2;
	data->valid_intersection = True;
	data->u = alpha;
	data->v = beta;
	data->t = lt;
}

static inline void	set_ss_quad_values(hit_record *rec, const ray *local_r,
		const t_item *self, t_qh *data)
{
	rec->u = data->u;
	rec->v = data->v;
	rec->mat = self->mat;
	set_face_normal(rec, local_r, v3(0, 1, 0));
	maybe_apply_perturb(rec);
	rec->normal = rotate3(rec->normal, self->transform.rot);
	rec->old_normal = rotate3(rec->old_normal, self->transform.rot);
}

Bool	hit_ss_quad(const ray *r, const interval ray_t, hit_record *rec,
		const t_item *self)
{
	ray		local_r;
	t_qh	data;
	PFPN	gt;

	local_r = apply_ray(r, self->bck);
	get_ss_quad_intersection(&local_r, &data);
	if (!data.valid_intersection)
		return (False);
	gt = t2global(data.t, &local_r, r, self->fwd);
	if (!contains(ray_t, gt))
		return (False);
	if (!is_interior(data.u, data.v))
		return (False);
	rec->t = gt;
	rec->p = ray_at(r, gt);
	set_ss_quad_values(rec, &local_r, self, &data);
	return (True);
}
