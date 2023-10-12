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

void	maybe_add_item(void)
{
	static t_item	*addin = NULL;
	static int		stage = 0;
	static vec3		base = (vec3){0};
	static vec3		anchor = (vec3){0};
	static vec3		total_move = (vec3){0};
	static vec3		mouse_prev = (vec3){0};

	if (v._1 || v._2 || v._3 || v._4 || v._0)
	{
		if (addin == NULL)
		{
			add_item(get_item_default());
			addin = &(v.items[v.item_count-1]);
		}
		else
		{
			*addin = get_item_default();
		}
		stage = 0;
		
		write(1, "WTFFFF", 7);
	}

	if (addin == NULL) return;

	if (v._space)
	{
		v._space = 0;
		total_move = v3(0);
		v._lclick = 1;
	}
	if (v._rclick)
	{
		remove_item(addin);
		v.selection_mode = NONE;
		addin = NULL;
		v._rclick = 0;
		return;
	}
	
	vec3 move = v3(v.mouse_pos.x - mouse_prev.x, v.mouse_pos.y - mouse_prev.y);
//	if (v._shift) move = v3(move.x/10, move.y/10};
	total_move = v3(total_move.x + move.x, total_move.y + move.y);
	v.selection_pos = addin->pos;
	vec3 selection_point = world_to_screenpos(addin->pos);

	if (stage == 0) //Init
	{
		stage = 1, total_move = v3(0);
		base = v3(0,0,0);
		anchor = v.mouse_pos;
		mouse_prev = v.mouse_pos;
		v.selection_mode = MOVE;
	}
	if (stage == 1) //Position
	{
		double aspect = (double)v.w/(double)v.h;
		vec3 uv = v3((v.mouse_pos.x)/v.h, (v.h-v.mouse_pos.y)/v.h, .2);
		vec3 uv2= v_scal(v_add(uv, v3(-.5*aspect,-.5)), 3);
		vec3 pos = reverse_project(uv2, v.vfov, 1);
		pos = rotate3(pos, v.camera_rot);
		pos = v3(total_move.x*3.0/v.w, -total_move.y*3.0/v.h, pos.x+pos.y);
		addin->pos = plane_alligned_add(base, v_scal(pos, 4));
		if (v._lclick)
		{
			stage = 2, v._lclick = 0, total_move = v3(0);
			base = addin->scale;
			anchor = v3(vec_dist(selection_point, v.mouse_pos));
			v.selection_mode = SCALE;
		}
	}
	if (stage == 2) //Scale
	{
		double d = vec_dist(selection_point, v.mouse_pos);
		addin->scale = plane_alligned_add(base, v_scal(v_3(d - anchor.x), 10.0/v.h));
		if (v._lclick)
		{
			stage = 3, v._lclick = 0, total_move = v3(0);
			base = addin->rot;
			anchor = v.mouse_pos;
			v.selection_mode = ROTATE;
		}
	}
	if (stage == 3) //Rotate
	{
		vec3 g = v3(anchor.x-selection_point.x, anchor.y-selection_point.y);
		double start_angle = atan2(g.y, g.x);
		double end_angle = atan2(g.y + total_move.y, g.x + total_move.x);
		//if (sign(v.mouse_pos.x-addin->pos.x) != sign(anchor.x-addin->pos.x)) end_angle += MYPI;
		addin->rot = plane_alligned_add(base, v3(-start_angle+end_angle, -start_angle+end_angle, -start_angle+end_angle));
		if (v._lclick)
		{
			stage = 5, v._lclick = 0;
			v.selection_mode = NONE;
		}
	}	
	//4 //pick color
	if (stage == 5) //forget the added one
	{
		addin = NULL;
		v.selection_mode = NONE;
	}
	
	mouse_prev = v.mouse_pos;
}
