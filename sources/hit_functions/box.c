
#include "fractol.h"

typedef struct s_bh
{
	PFPN			t;
	PFPN			u;
	PFPN			v;
	int				final_index;
	bool			valid_intersection;
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

bool	hit_box(const ray *r, const interval ray_t, hit_record *rec,
		const t_item *self)
{
	ray		local_r;
	t_bh	data;
	bool	valid_intersection;
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

static inline void	save_intersection_3(PFPN t[2], PFPN u[2], PFPN v[2],
		const ray *local_r)
{
	vec3	a;
	vec3	k;

	a = local_r->orig;
	k = local_r->dir;
	if (!close_enough(k.y))
	{
		t[4] = (a.y + 1.0) / -k.y;
		t[5] = (a.y - 1.0) / -k.y;
		u[4] = a.x + k.x * t[4];
		v[4] = a.z + k.z * t[4];
		u[5] = a.x + k.x * t[5];
		v[5] = a.z + k.z * t[5];
	}
	else
		(nohit(&t[4], &u[4], &v[4]), nohit(&t[5], &u[5], &v[5]));
}

static inline void	save_intersection_2(PFPN t[2], PFPN u[2], PFPN v[2],
		const ray *local_r)
{
	vec3	a;
	vec3	k;

	a = local_r->orig;
	k = local_r->dir;
	if (!close_enough(k.x))
	{
		t[2] = (a.x + 1.0) / -k.x;
		t[3] = (a.x - 1.0) / -k.x;
		u[2] = a.z + k.z * t[2];
		v[2] = a.y + k.y * t[2];
		u[3] = a.z + k.z * t[3];
		v[3] = a.y + k.y * t[3];
	}
	else
		(nohit(&t[2], &u[2], &v[2]), nohit(&t[3], &u[3], &v[3]));
	save_intersection_3(t, u, v, local_r);
}

// front_back
void	save_box_intersection(PFPN t[2], PFPN u[2], PFPN v[2],
		const ray *local_r)
{
	vec3	a;
	vec3	k;

	a = local_r->orig;
	k = local_r->dir;
	if (!close_enough(k.z))
	{
		t[0] = (a.z - 1.0) / -k.z;
		t[1] = (a.z + 1.0) / -k.z;
		u[0] = a.x + k.x * t[0];
		v[0] = a.y + k.y * t[0];
		u[1] = a.x + k.x * t[1];
		v[1] = a.y + k.y * t[1];
	}
	else
		(nohit(&t[0], &u[0], &v[0]), nohit(&t[1], &u[1], &v[1]));
	save_intersection_2(t, u, v, local_r);
}
