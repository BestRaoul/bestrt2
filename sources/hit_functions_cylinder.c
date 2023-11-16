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

typedef struct s_ch
{
	PFPN			t;
	PFPN			u;
	PFPN			v;
	int				final_index;
	Bool			valid_intersection;
}					t_ch;

static inline void	choose_final(PFPN t[4], PFPN u[4], PFPN v[4],
		t_ch *data)
{
	int	i;

	data->t = 100e6;
	data->final_index = 0;
	data->valid_intersection = False;
	i = 0;
	while (i < 4)
	{
		if (t[i] < data->t && t[i] > 0.0)
		{
			data->t = t[i];
			data->final_index = i;
			data->valid_intersection = True;
		}
		i++;
	}
	data->u = u[data->final_index];
	data->v = v[data->final_index];
}

static inline void	get_cylinder_intersection_2(PFPN t[4], vec3 av, vec3 kv)
{
	PFPN	a;
	PFPN	b;
	PFPN	c;
	PFPN	root;

	a = (kv.x * kv.x) + (kv.z * kv.z);
	b = 2.0 * (av.x * kv.x + av.z * kv.z);
	c = ((av.x * av.x) + (av.z * av.z)) - 1.0;
	root = sqrt((b * b) - 4.0 * a * c);
	if (root > 0.0)
	{
		t[0] = (-b + root) / (2 * a);
		t[1] = (-b - root) / (2 * a);
		if (fabs(av.y + kv.y * t[0]) > 1.0)
			t[0] = 100e6;
		if (fabs(av.y + kv.y * t[1]) > 1.0)
			t[1] = 100e6;
	}
	else
	{
		t[0] = 100e6;
		t[1] = 100e6;
	}
}

static inline void	get_cylinder_intersection(const ray *local_r, t_ch *data)
{
	PFPN	t[4];
	PFPN	u[4];
	PFPN	v[4];
	vec3	av;
	vec3	kv;

	av = local_r->orig;
	kv = local_r->dir;
	get_cylinder_intersection_2(t, av, kv);
	if (!close_enough(kv.y))
	{
		t[2] = (av.y - 1.0) / -kv.y;
		t[3] = (av.y + 1.0) / -kv.y;
		u[2] = av.x + kv.x * t[2];
		v[2] = av.z + kv.z * t[2];
		if (u[2] * u[2] + v[2] * v[2] > 1.0)
			t[2] = 100e6;
		u[3] = av.x + kv.x * t[3];
		v[3] = av.z + kv.z * t[3];
		if (u[3] * u[3] + v[3] * v[3] > 1.0)
			t[3] = 100e6;
	}
	else
		(set(&t[2], 100e6), set(&t[3], 100e6));
	choose_final(t, u, v, data);
}

static inline void	set_cylinder_values(hit_record *rec, const ray *local_r,
		const t_item *self, t_ch *data)
{
	vec3	local_p;
	vec3	outward_normal;

	local_p = ray_at(local_r, data->t);
	if (data->final_index <= 1)
	{
		outward_normal = v3(local_p.x, 0, local_p.z);
		rec->u = (atan2(local_p.z, local_p.x) + MYPI) / (2 * MYPI);
		rec->v = local_p.y / 2 + .5;
	}
	else
	{
		outward_normal = v3(0, 1 * (data->final_index == 2) - 1
				* (data->final_index == 3), 0);
		rec->u = .5 - data->u / 2;
		rec->v = .5 - data->v / 2;
	}
	rec->mat = self->mat;
	set_face_normal(rec, local_r, outward_normal);
	maybe_apply_perturb(rec);
	rec->normal = rotate3(rec->normal, self->transform.rot);
	rec->old_normal = rotate3(rec->old_normal, self->transform.rot);
}

Bool	hit_cylinder(const ray *r, const interval ray_t, hit_record *rec,
		const t_item *self)
{
	ray		local_r;
	t_ch	data;
	PFPN	gt;

	local_r = apply_ray(r, self->bck);
	get_cylinder_intersection(&local_r, &data);
	if (!data.valid_intersection)
		return (False);
	gt = t2global(data.t, &local_r, r, self->fwd);
	if (!contains(ray_t, gt))
		return (False);
	rec->t = gt;
	rec->p = ray_at(r, gt);
	set_cylinder_values(rec, &local_r, self, &data);
	return (True);
}
