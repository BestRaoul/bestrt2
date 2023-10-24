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

enum e_et {
	SET = 0,
	UPDATE,
	CONFIRM,
	RESET
};

int	set_reset(tfm *transform, vec3 *pmp, vec3 *init_p, int mode, int et)
{
	if (et == SET)
	{
		if (mode == MOVE)	*init_p = transform->pos;
		if (mode == ROTATE)	*init_p = transform->rot;
		if (mode == SCALE)	*init_p = transform->scale;
		*pmp = v.mouse_pos;
		v.selection_mode = mode;
		v.render_mode = RASTER;

		if (mode == MOVE)	set_cursor(XC_fleur);
		if (mode == ROTATE)	set_cursor(XC_exchange);
		if (mode == SCALE)	set_cursor(XC_sizing);

		return 1;
	}
	if (et == RESET)
	{
		if (mode == MOVE)	transform->pos = *init_p;
		if (mode == ROTATE)	transform->rot = *init_p;
		if (mode == SCALE)	transform->scale = *init_p;
		set_cursor(XC_crosshair);
		return 1;
	}
	return 0;
}

void	move_transform(tfm *transform, int et)
{
	const int	pixels_to_unit = 200;
	static vec3 pmp = (vec3){};
	static vec3 init_p = (vec3){};
	vec3 mp = v.mouse_pos;

	if (set_reset(transform, &pmp, &init_p, MOVE, et))
		return;

	//--- ACTUAL MOVE ---
	vec3 move = from_to(pmp, mp);
	if (v._shift) move = v_scal(move, 1./5.);
	move = v_scal(move, 1./pixels_to_unit);

	//	get_3d_move() -- complicated
	move = v3(move.x, 0, move.y);
	//	enc -- complicated

	transform->pos = plane_alligned_add(transform->pos, move);

	pmp = mp;
}

void	scale_transform(tfm *transform, int et)
{
	const int	pixels_to_unit = 200;
	static vec3 pmp = (vec3){};
	static vec3 init_p = (vec3){};
	vec3 mp = v.mouse_pos;

	if (set_reset(transform, &pmp, &init_p, SCALE, et))
		return;

	//--- ACTUAL MOVE ---
	vec3 move = from_to(pmp, mp);
	if (v._shift) move = v_scal(move, 1./5.);
	move = v_scal(move, 1./pixels_to_unit);

	//	get_3d_move() -- complicated
	move = v_3(v_len(move));
	//	enc -- complicated

	transform->scale = plane_alligned_add(transform->scale, move);

	pmp = mp;
}

void	rotate_transform(tfm *transform, int et)
{
	const int	pixels_to_unit = 200/MYPI;
	static vec3 pmp = (vec3){};
	static vec3 init_p = (vec3){};
	vec3 mp = v.mouse_pos;

	if (set_reset(transform, &pmp, &init_p, ROTATE, et))
		return;

	//--- ACTUAL MOVE ---
	vec3 move = from_to(pmp, mp);
	if (v._shift) move = v_scal(move, 1./5.);
	move = v_scal(move, 1./pixels_to_unit);

	//	get_3d_move() -- complicated
	move = v3(move.y, move.x, 0);
	//	enc -- complicated

	transform->rot = plane_alligned_add(transform->rot, move);

	pmp = mp;
}

void	mrs_null_reference(tfm *_, int et)
{
	printf("ERROR in MRS event :: null reference\n");
}

void (*apt[4])(tfm *, int) = {
	mrs_null_reference,
	move_transform,
	rotate_transform,
	scale_transform
};

void	manage_selection(void)
{
	if (v._lclick)
	{
		v._lclick = 0;
		v.selection_mode = NONE;

		ray r; hit_record rec;
		init_ray(v.mouse_pos.x, v.mouse_pos.y, &r);
		if (info_hit(&r, (interval){0, INFINITY}, &rec))
		{
			if (v.selected == &rec.item->transform)
			{
				v.lookat_toggle = 1; v.camera_change = 1; 
			}
			v.selected = &rec.item->transform;
			v.render_mode = RASTER;
			v.selection_mode = NONE;

			v.lookat = v.selected->pos;
			v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
		}
		else
			v.selected = NULL;
	}

	if (v.selected == NULL) return;


	if (v.selection_mode == NONE) // unselect
	{
		if (v._rclick)
		{
			v.selected = NULL;
			v._rclick = 0;
		}
		return;
	}

	if (v._space) //Confirm
	{
		v.selection_mode = NONE;
		v._space = 0;
		return;
	}
	if (v._rclick) //Reset
	{
		v._rclick = 0;
		apt[v.selection_mode](v.selected, RESET);
		v.selection_mode = NONE;
		return;
	}

	apt[v.selection_mode](v.selected, UPDATE);
}

void	maybe_add_item(void)
{
	if (v.selected != NULL) return;

	//item
	if (v._1 || v._2 || v._3 || v._4)
	{
		t_item *new_item = add_item_(get_item_default());
		v.selected = &new_item->transform;
		v.selection_is_item_lamp = True;
		
		move_transform(v.selected, SET);
		v.lookat = v.selected->pos; v.lookat_toggle = 1;
		v.camera_change = 1; v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
	}
	//lamp
	//lamp 6 (directional) : overides selection mode to rotate
	if (v._5 || v._6)
	{
		t_light *new_lamp = add_lamp_(get_lamp_default());
		v.selected = &new_lamp->transform;
		v.selection_is_item_lamp = False;

		move_transform(v.selected, SET);
		v.lookat = v.selected->pos; v.lookat_toggle = 1;
		v.camera_change = 1; v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
	}
return;

// static vec3		base = (vec3){0};
// static vec3		anchor = (vec3){0};
// static vec3		total_move = (vec3){0};
// static vec3		mouse_prev = (vec3){0};

// 	if (v._space)
// 	{
// 		v._space = 0;
// 		total_move = v3(0);
// 		v._lclick = 1;
// 	}
// 	if (v._rclick)
// 	{
// 		remove_item(addin);
// 		v.selection_mode = NONE;
// 		addin = NULL;
// 		v._rclick = 0;
// 		return;
// 	}
	
// 	vec3 move = v3(v.mouse_sc->scale.x - mouse_prev.x, v.mouse_pos.y - mouse_prev.y);
// //	if (v._shift) move = v3(move.x/10, move.y/10};
// 	total_move = v3(total_move.x + move.x, total_move.y + move.y);
// 	v.selection_pos = addin->pos;
// 	vec3 selection_point = world_to_screenpos(addin->pos);

// 	if (stage == 0) //Init
// 	{
// 		stage = 1, total_move = v3(0);
// 		base = v3(0,0,0);
// 		anchor = v.mouse_pos;
// 		mouse_prev = v.mouse_pos;
// 		v.selection_mode = MOVE;
// 	}
// 	if (stage == 1) //Position
// 	{
// 		double aspect = (double)v.w/(double)v.h;
// 		vec3 uv = v3((v.mouse_pos.x)/v.h, (v.h-v.mouse_pos.y)/v.h, .2);
// 		vec3 uv2= v_scal(v_add(uv, v3(-.5*aspect,-.5)), 3);
// 		vec3 pos = reverse_project(uv2, v.vfov, 1);
// 		pos = rotate3(pos, v.camera_rot);
// 		pos = v3(total_move.x*3.0/v.w, -total_move.y*3.0/v.h, pos.x+pos.y);
// 		addin->pos = plane_alligned_add(base, v_scal(pos, 4));
// 		if (v._lclick)
// 		{
// 			stage = 2, v._lclick = 0, total_move = v3(0);
// 			base = addin->scale;
// 			anchor = v3(vec_dist(selection_point, v.mouse_pos));
// 			v.selection_mode = SCALE;
// 		}
// 	}
// 	if (stage == 2) //Scale
// 	{
// 		double d = vec_dist(selection_point, v.mouse_pos);
// 		addin->scale = plane_alligned_add(base, v_scal(v_3(d - anchor.x), 10.0/v.h));
// 		if (v._lclick)
// 		{
// 			stage = 3, v._lclick = 0, total_move = v3(0);
// 			base = addin->rot;
// 			anchor = v.mouse_pos;
// 			v.selection_mode = ROTATE;
// 		}
// 	}
// 	if (stage == 3) //Rotate
// 	{
// 		vec3 g = v3(anchor.x-selection_point.x, anchor.y-selection_point.y);
// 		double start_angle = atan2(g.y, g.x);
// 		double end_angle = atan2(g.y + total_move.y, g.x + total_move.x);
// 		//if (sign(v.mouse_pos.x-addin->pos.x) != sign(anchor.x-addin->pos.x)) end_angle += MYPI;
// 		addin->rot = plane_alligned_add(base, v3(-start_angle+end_angle, -start_angle+end_angle, -start_angle+end_angle));
// 		if (v._lclick)
// 		{
// 			stage = 5, v._lclick = 0;
// 			v.selection_mode = NONE;
// 		}
// 	}	
// 	//4 //pick color
// 	if (stage == 5) //forget the added one
// 	{
// 		addin = NULL;
// 		v.selection_mode = NONE;
// 	}
	
// 	mouse_prev = v.mouse_pos;
}
