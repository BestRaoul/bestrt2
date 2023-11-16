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

char		*g_tone_names[9] = {
	"-- Raw --",
	"-- Gamma --",
	"-- Aces --",
	"-- HDR --",
	"-- Aces -> Gamma --",
	"-- HDR -> Gamma --",
	"-- Aces -> HDR -> Gamma --",
	"-- HDR -> Aces -> Gamma --",
};

static void	arrow_visualization(void)
{
	scribe("o", v3(v.w - 100, v.h - 100), GREEN);
	if (v._q)
		scribe("Q", v3(v.w - 109, v.h - 108), GREEN);
	if (v._e)
		scribe("E", v3(v.w - 92, v.h - 108), GREEN);
	if (v._up)
		scribe("^", v3(v.w - 100, v.h - 105), GREEN);
	if (v._down)
		scribe("v", v3(v.w - 100, v.h - 93), GREEN);
	if (v._right)
		scribe(">", v3(v.w - 93, v.h - 100), GREEN);
	if (v._left)
		scribe("<", v3(v.w - 107, v.h - 100), GREEN);
}

void	debug_ui(void)
{
	int		_x;
	int		_h;
	color	at_moose;

	if (!v._debug)
		return ;
	_x = v.w - 180;
	_h = 120;
	scribe_num("fov = %d", v.vfov, v3(_x, _h += 16), GREEN);
	scribe_num("depth = %d", v.max_depth, v3(_x, _h += 16), GREEN);
	scribe_v3d("CAMERA", v.camera_pos, v3(_x, _h += 16), GREEN);
	scribe_v3d("ROTATION", v.camera_rot, v3(_x, _h += 16), GREEN);
	if (v.lookat_toggle)
		scribe("LOCK -  ON", v3(_x, _h += 16), GREEN);
	else
		scribe("LOCK - OFF", v3(_x, _h += 16), GREEN);
	scribe(g_tone_names[v.tone_mode], v3(_x, _h += 16), GREEN);
	_h += 16;
	at_moose = rgb2color(get_pixel(v.mouse_pos.x, v.mouse_pos.y));
	if (v_eq(at_moose, BLACK))
		scribe_v3d("col:   ", at_moose, v3(_x, _h += 16), WHITE);
	else
		scribe_v3d("col:   ", at_moose, v3(_x, _h += 16), v_norm(at_moose));
	arrow_visualization();
}
