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

#define H_OFFSET 1.0
#define R_FACTOR 2.0

typedef struct s_ch
{
	PFPN			t;
	int				final_index;
	bool			valid_intersection;
}					t_ch;

static inline void	choose_final(PFPN t[3], t_ch *data)
{
	int	i;

	data->t = 100e6;
	data->final_index = 0;
	data->valid_intersection = False;
	i = 0;
	while (i < 3)
	{
		if (t[i] < data->t && t[i] > 0.0)
		{
			data->t = t[i];
			data->final_index = i;
			data->valid_intersection = True;
		}
		i++;
	}
}

static inline void	get_cone_intersection_2(PFPN t[3], vec3 av, vec3 kv)
{
	PFPN	a;
	PFPN	b;
	PFPN	root;
	PFPN	oi0;
	PFPN	oi1;

	a = (kv.x * kv.x) + (kv.z * kv.z) - (kv.y * kv.y);
	b = 2.0 * (av.x * kv.x + av.z * kv.z - av.y * kv.y);
	root = sqrt((b * b) - 4.0 * a
			* /* C */ ((av.x * av.x) + (av.z * av.z) - (av.y * av.y)));
	if (root > 0.0)
	{
		t[0] = (-b + root) / (2 * a);
		t[1] = (-b - root) / (2 * a);
		oi0 = av.y + kv.y * t[0];
		oi1 = av.y + kv.y * t[1];
		if (oi0 > 0.0 || oi0 < -1.0 * R_FACTOR)
			t[0] = 100e6;
		if (oi1 > 0.0 || oi1 < -1.0 * R_FACTOR)
			t[1] = 100e6;
	}
	else
		(set(&t[0], 100e6), set(&t[1], 100e6));
}

static inline void	get_cone_intersection(const ray *local_r, t_ch *data)
{
	PFPN	t[3];
	vec3	av;
	vec3	kv;
	PFPN	u;
	PFPN	v;

	av = v3(local_r->orig.x * R_FACTOR, local_r->orig.y - H_OFFSET,
			local_r->orig.z * R_FACTOR);
	kv = v3(local_r->dir.x * R_FACTOR, local_r->dir.y, local_r->dir.z
			* R_FACTOR);
	get_cone_intersection_2(t, av, kv);
	if (!close_enough(kv.y))
	{
		t[2] = (av.y + 1.0 * R_FACTOR) / -kv.y;
		u = av.x + kv.x * t[2];
		v = av.z + kv.z * t[2];
		if (u * u + v * v > 1.0 * (R_FACTOR * R_FACTOR))
			t[2] = 100e6;
	}
	else
	{
		t[2] = 100e6;
	}
	choose_final(t, data);
}

static inline void	set_cone_values(hit_record *rec, const ray *local_r,
		const t_item *self, t_ch *data)
{
	vec3	local_p;
	vec3	outward_normal;

	local_p = ray_at(local_r, data->t);
	if (data->final_index <= 1)
	{
		outward_normal.x = local_p.x;
		outward_normal.z = local_p.z;
		outward_normal.y = sqrtf((local_p.x * local_p.x) + (local_p.z
					* local_p.z)) / R_FACTOR;
		rec->u = (atan2(local_p.z, local_p.x) + MYPI) / (2 * MYPI);
		rec->v = local_p.y / 2 + .5;
	}
	else
	{
		outward_normal = v3(0, -1, 0);
		rec->u = .5 - local_p.x / 2;
		rec->v = .5 - local_p.z / 2;
	}
	rec->mat = self->mat;
	set_face_normal(rec, local_r, outward_normal);
	maybe_apply_perturb(rec);
	rec->normal = rotate3(rec->normal, self->transform.rot);
	rec->old_normal = rotate3(rec->old_normal, self->transform.rot);
}

bool	hit_cone(const ray *r, const interval ray_t, hit_record *rec,
		const t_item *self)
{
	ray		local_r;
	t_ch	data;
	PFPN	gt;

	local_r = apply_ray(r, self->bck);
	get_cone_intersection(&local_r, &data);
	if (!data.valid_intersection)
		return (False);
	gt = t2global(data.t, &local_r, r, self->fwd);
	if (!contains(ray_t, gt))
		return (False);
	rec->t = gt;
	rec->p = ray_at(r, gt);
	set_cone_values(rec, &local_r, self, &data);
	return (True);
}
