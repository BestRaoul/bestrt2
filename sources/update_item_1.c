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

void	(*g_apt[4])(tfm *, int) = {
	mrs_null_reference,
	move_transform,
	rotate_transform,
	scale_transform};

static Bool	shoot(void)
{
	ray			r;
	hit_record	rec;

	v._lclick = 0;
	default_crusor();
	if (v.selected && v.selection_mode != NONE)
		return (v.selection_mode = NONE, True);
	v.selection_mode = NONE;
	init_ray(v.mouse_pos.x, v.mouse_pos.y, &r);
	if (info_hit(&r, (interval){0, INFINITY}, &rec))
	{
		if (v.selected == &rec.item->transform)
		{
			v.lookat_toggle = 1;
			v.camera_change = 1;
		}
		v.selected = &rec.item->transform;
		v.render_mode = RASTER;
		v.selection_mode = NONE;
		v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
	}
	else
		v.selected = NULL;
	return (False);
}

static Bool	unselect_confirm_or_reset(void)
{
	if (v.selection_mode == NONE)
	{
		if (v._rclick)
		{
			v.selected = NULL;
			v._rclick = 0;
			default_crusor();
		}
		return (True);
	}
	if (v._space)
	{
		v.selection_mode = NONE;
		v._space = 0;
		return (default_crusor(), True);
	}
	if (v._rclick)
	{
		v._rclick = 0;
		g_apt[v.selection_mode](v.selected, RESET);
		v.selection_mode = NONE;
		return (default_crusor(), True);
	}
	return (False);
}

void	manage_selection(void)
{
	if (v._lclick && shoot())
		return ;
	if (v.selected == NULL)
		return ;
	if (unselect_confirm_or_reset())
		return ;
	g_apt[v.selection_mode](v.selected, UPDATE);
}

void	maybe_add_item(void)
{
	t_item	*new_item;
	t_light	*new_lamp;

	if (v.selected != NULL)
		return ;
	if (v._1 || v._2 || v._3 || v._4)
	{
		new_item = add_item_(get_item_default());
		v.selected = &new_item->transform;
		v.selection_is_item_lamp = True;
		move_transform(v.selected, SET);
		v.camera_change = 1;
		v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
	}
	if (v._5 || v._6)
	{
		new_lamp = add_lamp_(get_lamp_default());
		v.selected = &new_lamp->transform;
		v.selection_is_item_lamp = False;
		move_transform(v.selected, SET);
		v.camera_change = 1;
		v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
	}
}
