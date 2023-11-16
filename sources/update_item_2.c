/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upate_item.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

typedef struct s_mt_data {
	vec3	pg;
	PFPN	dy;
	vec3	ir;
	vec3	ib;
}	t_mt_data;

void	move_transform(tfm *transform, int et)
{
	static vec3	initial_position = (vec3){};
	static vec3	pmp = (vec3){};
	ri			r;
	t_mt_data	data;

	if (set_reset(transform, &initial_position, MOVE, et))
	{
		pmp = v.mouse_pos;
		return ;
	}
	r = get_rotation_indicator(initial_position,
			world_to_screenpos(initial_position));
	draw_modify_axis(r, initial_position);
	data.pg = project_onto_line(r.g_bot, r.g_top, v.mouse_pos);
	data.pg.z = 0;
	data.dy = tddd(r.anchor, r.g_top, from_to(r.anchor, r.g_top));
	get_intersections(initial_position, v.mouse_pos, &data.ir, &data.ib);
	if (v_eq(pmp, v.mouse_pos))
		return ;
	transform->pos = plane_alligned_add(initial_position,
			v3(data.ir.x - initial_position.x,
				tddd(r.anchor, data.pg, from_to(r.anchor, r.g_top)) / data.dy,
				data.ib.z - initial_position.z));
	pmp = v.mouse_pos;
}
// if (v._ctrl) v_round(&transform->pos, 0.25);

void	scale_transform(tfm *transform, int et)
{
	static vec3	initial_scale = (vec3){};
	static vec3	inital_mp = (vec3){};
	ri			r;
	PFPN		initial_d;
	PFPN		d;

	if (set_reset(transform, &initial_scale, SCALE, et))
	{
		inital_mp = v.mouse_pos;
		return ;
	}
	r = get_rotation_indicator(transform->pos,
			world_to_screenpos(transform->pos));
	draw_modify_axis(r, transform->pos);
	initial_d = vec_dist(r.anchor, inital_mp);
	d = vec_dist(r.anchor, v.mouse_pos);
	gizmo_line(r.anchor, v.mouse_pos, WHITE);
	transform->scale = plane_alligned_add(initial_scale,
			v_sub(v_scal(initial_scale, d / initial_d), initial_scale));
}
// if (v._ctrl) v_round(&transform->scale, 0.1);

void	rotate_transform(tfm *transform, int et)
{
	static vec3	initial_rot = (vec3){};
	static vec3	inital_mp = (vec3){};
	ri			r;
	vec3		initial_dir;
	vec3		dir;

	if (set_reset(transform, &initial_rot, ROTATE, et))
	{
		inital_mp = v.mouse_pos;
		return ;
	}
	r = get_rotation_indicator(transform->pos,
			world_to_screenpos(transform->pos));
	draw_modify_axis(r, transform->pos);
	initial_dir = from_to(r.anchor, inital_mp);
	dir = from_to(r.anchor, v.mouse_pos);
	gizmo_line(r.anchor, v.mouse_pos, WHITE);
	transform->rot = plane_alligned_add(initial_rot,
			v_3(atan2(dir.y, dir.x) + MYPI
				- atan2(initial_dir.y, initial_dir.x) + MYPI));
}
// if (v._ctrl) v_round(&transform->rot, 5*DEG2RAD);

void	mrs_null_reference(tfm *_, int et)
{
	printf("ERROR in MRS event :: null reference\n");
}
