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

#define X_ENABLED (v.plane != YZ && v.plane != Y && v.plane != Z)
#define Y_ENABLED (v.plane != XZ && v.plane != X && v.plane != Z)
#define Z_ENABLED (v.plane != XY && v.plane != X && v.plane != Y)

void	rotation_indicator(void)
{
	double dist_to = 1;
	double d = 0.06;
	color	line_color = WHITE;

	vec3 anchor = v_add(v.camera_pos, rotate3(v3(0,0,1), v.camera_rot));
	vec3 anchor_p = world_to_screenpos(anchor);
	vec3 r_top = world_to_screenpos(v_add(anchor, v3( d)));
	vec3 r_bot = world_to_screenpos(v_add(anchor, v3(-d)));
	vec3 g_top = world_to_screenpos(v_add(anchor, v3(0, d)));
	vec3 g_bot = world_to_screenpos(v_add(anchor, v3(0,-d)));
	vec3 b_top = world_to_screenpos(v_add(anchor, v3(0,0, d)));
	vec3 b_bot = world_to_screenpos(v_add(anchor, v3(0,0,-d)));

	const vec3 end = v3(v.w - 75, 75);
	vec3 move = v_sub(end, anchor_p);
	anchor_p = v_add(anchor_p, move);
	r_top = v_add(r_top, move);
	r_bot = v_add(r_bot, move);
	g_top = v_add(g_top, move);
	g_bot = v_add(g_bot, move);
	b_top = v_add(b_top, move);
	b_bot = v_add(b_bot, move);

	gizmo_dot(anchor_p, v3(.8, .6, .1));
	if (X_ENABLED)
	{
		gizmo_line(r_top, anchor_p, RED);
		gizmo_dot(r_top, RED);
		gizmo_dot(r_bot, v3(1, .2, .2));
		scribe("X", r_top.x+5, r_top.y-5, WHITE);
	// scribe("-X", r_bot.x+0, r_bot.y-5, WHITE);
	}
	if (Y_ENABLED)
	{
		gizmo_line(g_top, anchor_p, GREEN);
		gizmo_dot(g_top, GREEN);
		gizmo_dot(g_bot, v3(.2, 1, .2));
		scribe("Y", g_top.x+5, g_top.y-5, WHITE);
	// scribe("-Y", g_bot.x+0, g_bot.y-5, WHITE);
	}
	if (Z_ENABLED)
	{
		gizmo_line(b_top, anchor_p, BLUE);
		gizmo_dot(b_top, BLUE);
		gizmo_dot(b_bot, v3(.2, .2, 1));
		scribe("Z", b_top.x+5, b_top.y-5, WHITE);
	// scribe("-Z", b_bot.x+0, b_bot.y-5, WHITE);
	}
}

char	*mat_debugmode_names[9] = {
	"-- MAT (normal) --",
	"-- MAT (mist) --",

	"-- MAT (diffuse light) --",
	"-- MAT (diffuse color) --",
	"-- MAT (specular light) --",
	"-- MAT (specular color) --",

	"-- MAT (emission) --",
	"-- MAT (environement) --",
	"-- MAT (combined) --",
};

void	mode_name(void)
{
	double px = v.w/2.0 - 50, py = 20;
	switch (v.render_mode)	{
	case RASTER: scribe("-- Raster --", px, py, GREEN); break;
	case RAYTRACE: scribe("-- RAYz --", px, py, GREEN); break;
	case RAYTRACE_STEPS: scribe("-- step by step --", px, py, GREEN); break;
	case RAYTRACE_STEPS_2: scribe("-- step by step 2 --", px, py, GREEN); break;
	case RAYTRACE_STEPS_3: scribe("-- step by step 3 --", px, py, GREEN); break;
	case RAYTRACE_UVS: scribe("-- UVs --", px, py, GREEN); break;
	case RAYTRACE_DIST: scribe("-- DIST --", px, py, GREEN); break;
	case RAYTRACE_MAT_DEBUG: scribe(mat_debugmode_names[v.mat_debugmode], px, py, GREEN); break;
	default: scribe("-- wtf?!! --", px, py, GREEN); break;
	}
}

void	help_ui(void)
{
	mode_name();
	rotation_indicator();

	if (!v._help)
		return ;

	scribe_num("FPS %d", (int)(1.0/v.delta_time), 10, 10, new_color(.9, .8, 0));
	

	int	_h = v.h - 320;
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

void	debug_ui(void)
{
	if (!v._debug)
		return ;

	int _x = v.w - 200;
	int _h = 50;

	// scribe("DEBUG MODE", _x, _h+=16, GREEN);
	scribe_num("fov = %d", v.vfov, _x, _h+=16, GREEN);
	scribe_v3d("CAMERA", v.camera_pos, _x, _h+=16, GREEN);
	scribe_v3d("ROTATION", v.camera_rot, _x, _h+=16, GREEN);
	scribe(v.lookat_toggle?"LOCK - ON":"LOCK - OFF", _x, _h+=16, GREEN);

	_h+=16;
	color at_moose = rgb2color(get_pixel(v.mouse_pos.x, v.mouse_pos.y));
	scribe_v3d("col:   ",at_moose, _x, _h+=16, v_norm(at_moose));
	
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
