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

typedef struct s_bh
{
	PFPN			t;
	PFPN			u;
	PFPN			v;
	int				final_index;
	Bool			valid_intersection;
}					t_bh;

static inline void	set_box_values(hit_record *rec, const ray *local_r,
		const t_item *self, t_bh *data)
{
	data->u = .5 - data->u / 2;
	data->v = .5 - data->v / 2;
	rec->u = data->u;
	rec->v = data->v;
	rec->mat = self->mat;
	set_face_normal(rec, local_r, get_cube_normal(data->final_index, rec));
	maybe_apply_perturb(rec);
	rec->normal = rotate3(rec->normal, self->transform.rot);
	rec->old_normal = rotate3(rec->old_normal, self->transform.rot);
}

static inline void	choose_final(PFPN t[6], PFPN u[6], PFPN v[6],
	t_bh *data)
{
	PFPN	alpha;
	PFPN	beta;
	int		i;

	data->t = 100e6;
	data->final_index = 0;
	data->valid_intersection = False;
	i = 0;
	while (i < 6)
	{
		if (t[i] < data->t && t[i] > 0.0)
		{
			alpha = .5 - u[i] / 2;
			beta = .5 - v[i] / 2;
			if (is_interior(alpha, beta))
			{
				data->t = t[i];
				data->final_index = i;
				data->valid_intersection = True;
			}
		}
		i++;
	}
	data->u = u[data->final_index];
	data->v = v[data->final_index];
}

static inline void	get_box_intersection(const ray *local_r, t_bh *data)
{
	PFPN	t[6];
	PFPN	u[6];
	PFPN	v[6];

	save_box_intersection(t, u, v, local_r);
	choose_final(t, u, v, data);
}

Bool	hit_box(const ray *r, const interval ray_t, hit_record *rec,
		const t_item *self)
{
	ray		local_r;
	t_bh	data;
	Bool	valid_intersection;
	PFPN	gt;

	local_r = apply_ray(r, self->bck);
	get_box_intersection(&local_r, &data);
	if (!data.valid_intersection)
		return (False);
	gt = t2global(data.t, &local_r, r, self->fwd);
	if (!contains(ray_t, gt))
		return (False);
	rec->t = gt;
	rec->p = ray_at(r, gt);
	set_box_values(rec, &local_r, self, &data);
	return (True);
}
