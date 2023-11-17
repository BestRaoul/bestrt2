/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	handle_submode_next(void)
{
	if (v.render_mode == RAYTRACE_MAT_DEBUG
		|| v.render_mode == RAYTRACE_MAT_VALUES || v.render_mode == RAYTRACE
		|| v.render_mode == RAYTRACE_STEPS)
		v._rerender = 1;
	if (v.render_mode == RAYTRACE_MAT_DEBUG)
		v.mat_debugmode = (v.mat_debugmode + 1) % MAT_DEBUGMODES_MAX;
	if (v.render_mode == RAYTRACE_MAT_VALUES)
		v.mat_valuesmode = (v.mat_valuesmode + 1) % MAT_VALUESMODES_MAX;
	if (v.render_mode == RAYTRACE || v.render_mode == RAYTRACE_STEPS)
		v.tone_mode = (v.tone_mode + 1) % TONEMODES_MAX;
}

static bool	ignore_our_keypresses(int k)
{
	return ((k == K_ESC) || (k == K_TAB) || (k == K_MOVE) || (k == K_ROTATE)
		|| (k == K_SCALE) || (k == K_RENDERMODE) || (k == K_MATMODE)
		|| (k == K_LOCKON) || (k == K_MOTION) || (k == K_HELPON) || (k == K_0)
		|| (k == K_1) || (k == K_2) || K_3) || (k == K_4) || (k == K_5)
		|| (k == K_6) || (k == K_7) || (k == K_8) || (k == K_9) || (k == K_NP_0)
		|| (k == K_NP_1) || (k == K_NP_2) || (k == K_NP_3) || (k == K_NP_4)
		|| (k == K_NP_5) || (k == K_NP_6) || (k == K_NP_7) || (k == K_NP_8)
		|| (k == K_NP_9) || (k == K_REPRINT) || (k == K_UP) || (k == K_DOWN)
		|| (k == K_LEFT) || (k == K_RIGHT) || (k == K_Q) || (k == K_E)
		|| (k == K_SPACE) || (k == K_SHIFT) || (k == K_CTRL) || (k == K_X)
		|| (k == K_Y) || (k == K_Z);
}

/*[wasd]*/
int	handle_key_release(int k)
{
	if (k == K_UP)
		v._up = 0;
	else if (k == K_DOWN)
		v._down = 0;
	else if (k == K_LEFT)
		v._left = 0;
	else if (k == K_RIGHT)
		v._right = 0;
	else if (k == K_Q)
		v._q = 0;
	else if (k == K_E)
		v._e = 0;
	else if (k == K_SPACE)
		v._space = 0;
	else if (k == K_SHIFT)
		v._shift = 0;
	else if (k == K_CTRL)
		v._ctrl = 0;
	else if (k == K_PLUS || k == K_MINUS)
		v.max_depth += (k == K_PLUS) - (k == K_MINUS);
	else if (!ignore_our_keypresses(k))
		printf("[UP] key <%d>\n", k);
	return (0);
}

// v._7 = 0; v._8 = 0;//, v._9 = 0;
void	_reset_consumable_clicks(void)
{
	v._lclick = 0;
	v._llift = 0;
	v._rclick = 0;
	v._rlift = 0;
	v._0 = 0;
	v._1 = 0;
	v._2 = 0;
	v._3 = 0;
	v._4 = 0;
	v._5 = 0;
	v._6 = 0;
	v._np0 = 0;
	v._np1 = 0;
	v._np2 = 0;
	v._np3 = 0;
	v._np4 = 0;
	v._np5 = 0;
	v._np6 = 0;
	v._np7 = 0;
	v._np8 = 0;
	v._np9 = 0;
	v._p = 0;
}
