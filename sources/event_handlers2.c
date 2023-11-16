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

int	handle_key_press2(int k);
int	handle_key_press3(int k);
int	handle_key_press4(int k);
int	handle_key_press5(int k);

int	handle_key_press(int k)
{
	if (k == K_ESC)
		exit(0);
	else if (k == K_TAB)
	{
		if (v._shift)
			render_movie();
		else
			write_img();
		clear_img(v.img);
	}
	else if (k == K_MOVE)
		move_transform(v.selected, 0);
	else if (k == K_ROTATE)
		rotate_transform(v.selected, 0);
	else if (k == K_SCALE)
		scale_transform(v.selected, 0);
	else if (k == K_RENDERMODE)
	{
		v._rerender = 1;
		v.render_mode = (v.render_mode + 1) % RENDERMODES_MAX;
	}
	else
		return (handle_key_press2(k));
	return (0);
}

int	handle_key_press2(int k)
{
	if (k == K_MATMODE)
		handle_submode_next();
	else if (k == K_LOCKON)
		v.lookat_toggle = !v.lookat_toggle;
	else if (k == K_MOTION)
		v.motion_enabled = !v.motion_enabled;
	else if (k == K_HELPON)
	{
		if (v._shift)
			v._debug = !v._debug;
		else
			v._help = !v._help;
	}
	else
		return (handle_key_press3(k));
	return (0);
}

int	handle_key_press3(int k)
{	
	if (k == K_0)
		v._0 = 1;
	else if (k == K_1)
		v._1 = 1;
	else if (k == K_2)
		v._2 = 1;
	else if (k == K_3)
		v._3 = 1;
	else if (k == K_4)
		v._4 = 1;
	else if (k == K_5)
		v._5 = 1;
	else if (k == K_6)
		v._6 = 1;
	else if (k == K_7)
		v._7 = 1;
	else if (k == K_8)
		v._8 = 1;
	else if (k == K_9)
		v._9 = 1;
	else
		return (handle_key_press4(k));
	return (0);
}

int	handle_key_press4(int k)
{
	if (k == K_NP_0)
		v._np0 = 1;
	else if (k == K_NP_1)
		v._np1 = 1;
	else if (k == K_NP_2)
		v._np2 = 1;
	else if (k == K_NP_3)
		v._np3 = 1;
	else if (k == K_NP_4)
		v._np4 = 1;
	else if (k == K_NP_5)
		v._np5 = 1;
	else if (k == K_NP_6)
		v._np6 = 1;
	else if (k == K_NP_7)
		v._np7 = 1;
	else if (k == K_NP_8)
		v._np8 = 1;
	else if (k == K_NP_9)
		v._np9 = 1;
	else if (k == K_REPRINT)
		v._p = 1;
	else
		return (handle_key_press5(k));
	return (0);
}

// printf("[DOWN] key <%d>\n", k);
int	handle_key_press5(int k)
{
	if (k == K_UP)
		v._up = 1;
	else if (k == K_DOWN)
		v._down = 1;
	else if (k == K_LEFT)
		v._left = 1;
	else if (k == K_RIGHT)
		v._right = 1;
	else if (k == K_Q)
		v._q = 1;
	else if (k == K_E)
		v._e = 1;
	else if (k == K_SPACE)
		v._space = 1;
	else if (k == K_SHIFT)
		v._shift = 1;
	else if (k == K_CTRL)
		v._ctrl = 1;
	else if (k == K_X)
		plane_switch(X, YZ);
	else if (k == K_Y)
		plane_switch(Y, XZ);
	else if (k == K_Z)
		plane_switch(Z, XY);
	return (0);
}
