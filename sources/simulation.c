/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raster_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

typedef struct s_sim_hit
{
	bool		simd;
	vec3		p;
	color		c;
	hit_record	rec;
}	t_sim_hit;

void	simulate_ray(ray *r, t_sim_hit *hits, int depth)
{
	hit_record	rec;
	bool		did_hit;
	ray			scattered;
	bool		_;
	int			bounce;

	hits[0] = (t_sim_hit){True, r->orig, WHITE};
	bounce = 1;
	while (bounce < depth)
		hits[bounce++].simd = False;
	bounce = 1;
	while (bounce < depth)
	{
		rec = (hit_record){};
		did_hit = hit(r, (interval){0.001, INFINITY}, &rec);
		hits[bounce] = (t_sim_hit){True, v_add(r->orig, v_scal(r->dir, .2)),
			ERROR_CYAN, rec};
		if (!did_hit)
			break ;
		pbr_scatter(r, &rec, &scattered, &_);
		hits[bounce].p = scattered.orig;
		hits[bounce].c = rec.mat.base_color.color_value;
		*r = scattered;
		bounce++;
	}
}

void	draw_rayzz(t_sim_hit *hits, int depth)
{
	t_sim_hit	previous_hit;
	t_sim_hit	*h;
	t_light		*l;
	int			bounce;

	previous_hit = hits[0];
	bounce = 1;
	while (bounce < depth && hits[bounce].simd)
	{
		h = &(hits[bounce]);
		draw_projected_dot(h->p, h->c);
		draw_projected_line(previous_hit.p, h->p, previous_hit.c);
		previous_hit = *h;
		if (bounce + 1 < depth)
			draw_projected_line(h->p, hits[bounce + 1].p, h->c);
		draw_projected_line(h->p, v_add(h->p, v_scal(h->rec.normal, .4)),
			h->rec.normal);
		bounce++;
	}
}

/*
continue ;
if (v.light_count > 0)
{
	l = &(v.lights[0]);
	to_light = (ray){h->p, v_norm(from_to(h->p, l->transform.pos))};
	did_hit = hit(&to_light, (interval){0.0001,
			vec_dist(l->transform.pos, h->p)}, &rec);
	oi = h->p;
	if (did_hit)
	{
		draw_projected_dot(rec.p, RED);
		draw_projected_line(h->p, rec.p, RED);
		oi = rec.p;
	}
	draw_projected_line(oi, l->transform.pos, WHITE);
}
*/

void	simulate_rayzz(void)
{
	int					depth;
	static t_sim_hit	hits[100 + 1];
	ray					r;

	depth = v.max_depth;
	if (depth > 100)
	{
		write(1, "Careful simulation depth is capped at 100, \
		modify code if needed..\n", 68);
		depth = 100;
	}
	if (v._ctrl)
	{
		init_ray(v.mouse_pos.x, v.mouse_pos.y, &r);
		simulate_ray(&r, hits, depth);
	}
	draw_rayzz(hits, depth);
}

/*
{

	for (int i=0; i<=sim_depth; i++)
	{
		draw_projected_dot(hits[i].p, c);
		if (i!= sim_depth)
		{
			draw_projected_line(hits[i].p, hits[i+1].p, c);
			draw_projected_line(hits[i].p,
				v_add(hits[i].p, hits[i].rec.normal), BLUE);
			draw_projected_dot(hits[i+1].p, RED);
		}
		c = hits[i].c;
		c = WHITE;
		if (c.z)
		{
			//draw_projected_dot(hits[i+1].p, c);
			draw_projected_line(hits[i+1].p, hits[i+2].p, c);
			break ;
		}
	}



}

{
	static hit_record rec = (hit_record){};
	static bool	did_hit = False;
	static vec3 old_normal;
	static vec3 new_normal;

	if (v._shift)
	{
		ray shoot;
		init_ray(v.mouse_pos.x, v.mouse_pos.y, &shoot);
		did_hit = hit(&shoot, (interval){0.001, INFINITY}, &rec);

		old_normal = rec.normal;
		// maybe_apply_perturb(&rec);
		// new_normal = rec.normal;
	}
	if (did_hit)
	{
		vec3 from = rec.p; from = world_to_screenpos(from);
		vec3 to_old = v_add(rec.p, v_scal(old_normal, .2));
		to_old = world_to_screenpos(to_old);
		vec3 to_old_h = v_add(rec.p, v_scal(old_normal, .1));
		to_old_h = world_to_screenpos(to_old_h);
		vec3 to_new = v_add(rec.p, v_scal(new_normal, .2));
		to_new = world_to_screenpos(to_new);

		color pink = v3(1, .2, 0);
		color green = evaluate(&(rec.mat.normal), rec.u, rec.v);
		draw_debug_dot(from, pink);
		draw_debug_dot(to_old, pink);
		draw_debug_dot(to_old_h, pink);
		draw_debug_line(from, to_old, pink);

		draw_debug_dot(to_new, green);
		draw_debug_line(from, to_new, green);
	}
}
*/
