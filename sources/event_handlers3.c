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
		|| v.render_mode == RAYTRACE_MAT_VALUES
		|| v.render_mode == RAYTRACE
		|| v.render_mode == RAYTRACE_STEPS)
		v._rerender = 1;
	if (v.render_mode == RAYTRACE_MAT_DEBUG)
		v.mat_debugmode = (v.mat_debugmode + 1) % MAT_DEBUGMODES_MAX;
	if (v.render_mode == RAYTRACE_MAT_VALUES)
		v.mat_valuesmode = (v.mat_valuesmode + 1) % MAT_VALUESMODES_MAX;
	if (v.render_mode == RAYTRACE
		|| v.render_mode == RAYTRACE_STEPS)
		v.tone_mode = (v.tone_mode + 1) % TONEMODES_MAX;
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
	else
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
