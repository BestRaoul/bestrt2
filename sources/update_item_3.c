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

int	set_reset(tfm *transform, vec3 *init_p, int mode, int et)
{
	if (et == SET)
	{
		if (v.selected == NULL)
			return (1);
		if (mode == MOVE)
			*init_p = transform->pos;
		if (mode == ROTATE)
			*init_p = transform->rot;
		if (mode == SCALE)
			*init_p = transform->scale;
		v.selection_mode = mode;
		v.render_mode = RASTER;
		return (mode_cursor(mode), 1);
	}
	if (et == RESET)
	{
		if (mode == MOVE)
			transform->pos = *init_p;
		if (mode == ROTATE)
			transform->rot = *init_p;
		if (mode == SCALE)
			transform->scale = *init_p;
		return (default_crusor(), 1);
	}
	return (0);
}

// should be on top
void	draw_modify_axis(ri r, vec3 inital_position)
{
	vec3	f;
	vec3	t;

	if (v.selection_mode != NONE && v.plane != XYZ)
	{
		if (X_ENABLED)
		{
			f = v_add(inital_position, v3(v.far));
			t = v_add(inital_position, v3(-v.far));
			draw_projected_line(f, t, X_COLOR);
		}
		if (Y_ENABLED)
		{
			f = v_add(inital_position, v3(0, v.far));
			t = v_add(inital_position, v3(0, -v.far));
			draw_projected_line(f, t, Y_COLOR);
		}
		if (Z_ENABLED)
		{
			f = v_add(inital_position, v3(0, 0, v.far));
			t = v_add(inital_position, v3(0, 0, -v.far));
			draw_projected_line(f, t, Z_COLOR);
		}
	}
}

void	get_intersections(vec3 pos, vec3 mp, vec3 *ir, vec3 *ib)
{
	t_item		temp;
	hit_record	rec;
	ray			raii;

	temp = (t_item){(tfm){pos, v3(), v_3(1)}, new_m(BLACK), NULL, hit_plane};
	set_transform_matrix(&temp.transform, temp.fwd, temp.bck);
	init_ray(mp.x, mp.y, &raii);
	if (hit_plane(&raii, INTERVAL_FORWARD, &rec, &temp))
	{
		*ir = v3(rec.p.x, pos.y, pos.z);
		*ib = v3(pos.x, pos.y, rec.p.z);
	}
	else
	{
		*ir = pos;
		*ib = pos;
	}
}
