/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	rotation_indicator(void)
{
	vec3	x = v3(40);
	vec3	y = v3(0,-40);
	vec3	z = v3(0,0,40);

	vec3	show = v3(v.w - 100, 100);
	x = rotate3(x, v.camera_rot);
	y = rotate3(y, v.camera_rot);
	z = rotate3(z, v.camera_rot);
	gizmo_line(show, v_add(show, x), RED);
	gizmo_line(show, v_add(show, y), GREEN);
	gizmo_line(show, v_add(show, z), BLUE);
	gizmo_dot(v_add(show, x), RED);
	gizmo_dot(v_add(show, y), GREEN);
	gizmo_dot(v_add(show, z), BLUE);

	gizmo_dot(v_add(show, v_scal(x, -1)), new_color(1, .2, .2));
	gizmo_dot(v_add(show, v_scal(y, -1)), new_color(.2, 1, .2));
	gizmo_dot(v_add(show, v_scal(z, -1)), new_color(.2, .2, 1));
}

void	help_ui(void)
{
	//rotation_indicator();
	if (!v._help)
		return ;

	scribe_num("FPS %d", (int)(1.0/v.delta_time), 10, 10, new_color(.9, .8, 0));
	

	int	_h = v.h - 350;
	int	_x = 30;
	color	_c = new_color(.9, .8, 0);

	scribe("[INSERT HELP PANEL HERE]",	_x, _h+=16, _c);
	scribe("Help        -H",			_x, _h+=16, _c);
	scribe("Debug       -^H",			_x, _h+=16, _c);
	scribe("Rerender    -R",			_x, _h+=16, _c);
	scribe("Print       -P",			_x, _h+=16, _c);
	_h+=16;
	scribe("-- Default --",				_x, _h+=16, _c);
	scribe("Move    -wasd.arrows.space^",_x, _h+=16, _c);
	scribe("Mouse   -Rotate Head",		_x, _h+=16, _c);
	scribe("numpad  -Roll..",			_x, _h+=16, _c);
	_h+=16;
	scribe("-- Add Item --",			_x, _h+=16, _c);
	scribe("numpad  -add shape",		_x, _h+=16, _c);
	_h+=16;
	scribe("-- Selection --",			_x, _h+=16, _c);
	scribe("S.M.R   -Scale Move Rotate",_x, _h+=16, _c);
	scribe("numpad  -Orbit..",			_x, _h+=16, _c);
	scribe("X.Y.Z   -Planes Toggle",	_x, _h+=16, _c);
	scribe("?space? -default value",	_x, _h+=16, _c);
	
}

void	draw_light_gizmo(t_light *l)
{
	if (l->is_dir)
	{
		vec3 p = v3(0, 3, 0);
		vec3 p2 = v_add(p, l->pos_dir);
		p = world_to_screenpos(p);
		p2 = world_to_screenpos(p2);
		draw_debug_dot(p2, l->col);
		draw_debug_line(p, p2, l->col);
	}
	else
	{
		vec3 p = world_to_screenpos(l->pos_dir);
		draw_debug_dot(p, l->col);
	}
}

void	debug_ui(void)
{
	if (!v._debug)
		return ;

	for (int i=0; i<v.light_count; i++)
		draw_light_gizmo(&v.lights[i]);

	int _x = v.w - 200;
	int _h = 50;

	scribe("DEBUG MODE", _x, _h+=16, GREEN);
	scribe_num("fov=%d", v.vfov, _x, _h+=16, GREEN);
	scribe_v3d("CAMERA", v.camera_pos, _x, _h+=16, GREEN);
	scribe_v3d("ROTAT", v.camera_rot, _x, _h+=16, GREEN);
	scribe(v.lookat_toggle?"LOCK - ON":"LOCK - OFF", _x, _h+=16, GREEN);

	switch (v.render_mode)	{
	case RASTER: scribe("-- Raster --", _x, _h+=16, GREEN); break;
	case RAYTRACE: scribe("-- RAYz --", _x, _h+=16, GREEN); break;
	case RAYTRACE_STEPS: scribe("-- step by step --", _x, _h+=16, GREEN); break;
	case RAYTRACE_UVS: scribe("-- UVs --", _x, _h+=16, GREEN); break;
	case RAYTRACE_DIST: scribe("-- DIST --", _x, _h+=16, GREEN); break;
	default: scribe("-- wtf?!! --", _x, _h+=16, GREEN); break;
	}


	//plane draw
//	if (v.plane == XY || v.plane == X)
//		draw_debug_line(v3(0,v.h/2),v3(v.w,v.h/2), color(200, 0, 50));
//	if (v.plane == XY || v.plane == Y)
//		draw_debug_line(v3(v.w/2,0),v3(v.w/2,v.h), color(0, 200, 50));
	
	//mouse drag
	if (v._lmouse) gizmo_drag(v.lm_start_pos, v.mouse_pos, GREEN);
	if (v._rmouse) gizmo_drag(v.rm_start_pos, v.mouse_pos, GREEN);

	//center
	scribe("x", v.w/2 - 2, v.h/2+4 , GREEN);

	//arrows visualisation
	scribe("o", v.w - 100, v.h - 102, GREEN);
	if (v._up) scribe("^", v.w - 100, v.h - 105, GREEN);
	if (v._down) scribe("v", v.w - 100, v.h - 93, GREEN);
	if (v._right) scribe(">", v.w - 93, v.h - 100, GREEN);
	if (v._left) scribe("<", v.w - 107, v.h - 100, GREEN);
}
