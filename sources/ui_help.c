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

char		*g_mat_debugmode_names[9] = {
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

char		*g_mat_valuesmode_names[9] = {
	"-- VALUE (albedo) --",
	"-- VALUE (metalic) --",
	"-- VALUE (specular) --",
	"-- VALUE (roughness) --",
	"-- VALUE (transmission) --",
	"-- VALUE (transmission roughness) --",
	"-- VALUE (emission) --",
	"-- VALUE (normal) --",
	"-- VALUE (alpha) --",
};

ri	get_rotation_indicator(vec3 anchor, vec3 end)
{
	ri		r;
	vec3	move;

	r.anchor = world_to_screenpos(anchor);
	r.r_top = world_to_screenpos(v_add(anchor, v3(1)));
	r.r_bot = world_to_screenpos(v_add(anchor, v3(-1)));
	r.g_top = world_to_screenpos(v_add(anchor, v3(0, 1)));
	r.g_bot = world_to_screenpos(v_add(anchor, v3(0, -1)));
	r.b_top = world_to_screenpos(v_add(anchor, v3(0, 0, 1)));
	r.b_bot = world_to_screenpos(v_add(anchor, v3(0, 0, -1)));
	r.anchor.z = 0;
	r.r_top.z = 0;
	r.r_bot.z = 0;
	r.g_top.z = 0;
	r.g_bot.z = 0;
	r.b_top.z = 0;
	r.b_bot.z = 0;
	move = from_to(r.anchor, v3(end.x, end.y, 0));
	r.anchor = v_add(r.anchor, move);
	r.r_top = v_add(r.r_top, move);
	r.r_bot = v_add(r.r_bot, move);
	r.g_top = v_add(r.g_top, move);
	r.g_bot = v_add(r.g_bot, move);
	r.b_top = v_add(r.b_top, move);
	return (r.b_bot = v_add(r.b_bot, move), r);
}

static ri	ortho_ri(const vec3 anchor, const vec3 end, PFPN scale)
{
	ri		r;
	vec3	move;

	r.anchor = world_to_screenpos_ortho(anchor);
	r.r_top = world_to_screenpos_ortho(v_add(anchor, v3(scale)));
	r.r_bot = world_to_screenpos_ortho(v_add(anchor, v3(-scale)));
	r.g_top = world_to_screenpos_ortho(v_add(anchor, v3(0, scale)));
	r.g_bot = world_to_screenpos_ortho(v_add(anchor, v3(0, -scale)));
	r.b_top = world_to_screenpos_ortho(v_add(anchor, v3(0, 0, scale)));
	r.b_bot = world_to_screenpos_ortho(v_add(anchor, v3(0, 0, -scale)));
	move = v_sub(end, r.anchor);
	r.anchor = v_add(r.anchor, move);
	r.r_top = v_add(r.r_top, move);
	r.r_bot = v_add(r.r_bot, move);
	r.g_top = v_add(r.g_top, move);
	r.g_bot = v_add(r.g_bot, move);
	r.b_top = v_add(r.b_top, move);
	r.b_bot = v_add(r.b_bot, move);
	return (r);
}

static void	draw_rotation_indicator(void)
{
	const vec3	end = v3(v.w - 65, 70);
	ri			r;

	r = ortho_ri(v_add(v.camera_pos,
				rotate3(v3(0, 0, 1), v.camera_rot)), end, 0.1);
	draw_debug_dot(r.anchor, v3(.8, .6, .1));
	if (X_ENABLED)
	{
		draw_debug_line(r.r_top, r.anchor, X_COLOR);
		draw_debug_dot(r.r_top, X_COLOR);
		draw_debug_dot(r.r_bot, v3(1, .2, .2));
		scribe("X", v3(r.r_top.x + 5, r.r_top.y - 5), get_inverted(r.r_top));
	}
	if (Y_ENABLED)
	{
		(draw_debug_line(r.g_top, r.anchor, Y_COLOR), draw_debug_dot(r.g_top,
				Y_COLOR), draw_debug_dot(r.g_bot, v3(.2, 1, .2)));
		scribe("Y", v3(r.g_top.x + 5, r.g_top.y - 5), get_inverted(r.g_top));
	}
	if (Z_ENABLED)
	{
		(draw_debug_line(r.b_top, r.anchor, Z_COLOR), draw_debug_dot(r.b_top,
				Z_COLOR), draw_debug_dot(r.b_bot, v3(.2, .2, 1)));
		scribe("Z", v3(r.b_top.x + 5, r.b_top.y - 5), get_inverted(r.b_top));
	}
}

static void	scribe_mode_name(void)
{
	vec3	p;
	color	_c;

	p = v3((int)(v.w / 2.0 - 50), (int)20);
	_c = get_inverted(p);
	if (v.render_mode == RASTER)
		scribe("-- Raster --", p, _c);
	else if (v.render_mode == RAYTRACE)
		scribe("-- RAYz --", p, _c);
	else if (v.render_mode == RAYTRACE_STEPS)
		scribe("-- step by step --", p, _c);
	else if (v.render_mode == RAYTRACE_UVS)
		scribe("-- UVs --", p, _c);
	else if (v.render_mode == RAYTRACE_MAT_DEBUG)
		scribe(g_mat_debugmode_names[v.mat_debugmode], p, _c);
	else if (v.render_mode == RAYTRACE_MAT_VALUES)
		scribe(g_mat_valuesmode_names[v.mat_valuesmode], p, _c);
	else
		scribe("-- wtf?!! --", p, _c);
}

void	help_ui(void)
{
	const int	_x = 30;
	int			_h;
	color		_c;

	_c = new_color(.9, .8, 0);
	scribe_mode_name();
	draw_rotation_indicator();
	if (!v._help)
		return ;
	scribe_num("FPS %d", (int)(1.0 / v.delta_time), v3(10, 10), _c);
	_h = v.h - 260;
	scribe("Move        -arrows.Q.E", v3(_x, _h += 24), _c);
	scribe("numpad      -orbit and views", v3(_x, _h += 16), _c);
	scribe("Mode +      -O", v3(_x, _h += 16), _c);
	scribe("SubMode +   -P", v3(_x, _h += 16), _c);
	_h += 16;
	scribe("-- Items --", v3(_x, _h += 16), _c);
	scribe("1 to 4               -add shape", v3(_x, _h += 16), _c);
	scribe("5 or 6               -add lamp", v3(_x, _h += 16), _c);
	scribe("Move, Scale, Rotate  -G.S.R", v3(_x, _h += 16), _c);
	_h += 16;
	scribe("-- Other --", v3(_x, _h += 16), _c);
	scribe("X.Y.Z   -Planes Toggle", v3(_x, _h += 16), _c);
	scribe("Tab     -screenshot  .bmp", v3(_x, _h += 16), _c);
	scribe("^Tab    -render      .wbm", v3(_x, _h += 16), _c);
}
// scribe("[HELP PANEL]               ", v3(_x, _h += 16), _c);

// scribe("[INSERT HELP PANEL HERE]", v3(_x, _h += 16), _c);
// scribe("Help        -H", v3(_x, _h += 16), _c);
// scribe("Debug       -^H", v3(_x, _h += 16), _c);
