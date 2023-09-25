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

#ifdef LINUX
# define LCLICK 1
# define RCLICK 3
# define ZOOM_IN 4
# define ZOOM_OUT 5
#else
# define LCLICK 1
# define RCLICK 2
# define ZOOM_IN 5
# define ZOOM_OUT 4
#endif

int	handle_mouse_move(int x, int y)
{
	v.mouse_pos = v3(x, y);
	return (0);
	(void) x;
	(void) y;
}

int	handle_mouse_press(int button, int x, int y)
{
	if (button == LCLICK)
	{
		v._lmouse = 1;
		v._lclick = 1;
		v.lm_start_pos = v3(x, y);
	}
	else if (button == RCLICK)
	{
		v._rmouse = 1;
		v._rclick = 1;
		v.rm_start_pos = v3(x, y);
	}
	else if (button == ZOOM_IN)
		v._scroll += 1;
	else if (button == ZOOM_OUT)
		v._scroll -= 1;

	if (v._shift && (button == ZOOM_IN || button == ZOOM_OUT))
		set_cursor(v.mlx, v.win, v._scroll);
	return (0);
	(void) button;
	(void) x;
	(void) y;
}

int	handle_mouse_release(int button, int x, int y)
{
	if (button == LCLICK)
	{
		v._lmouse = 0;
		v._llift = 1;
	}
	else if (button == RCLICK)
	{
		v._rmouse = 0;
		v._rlift = 1;
	}
	return (0);
	(void) button;
	(void) x;
	(void) y;
}

void	plane_switch(enum e_plne _plane, enum e_plne _excluded)
{
	if(!v._shift)
		v.plane = _plane * (v.plane!=_plane);
	else
		v.plane = _excluded * (v.plane!=_excluded);
}

int	handle_key_press(int k)
{
	switch (k)
	{
		case K_ESC: exit(0); break;
		case K_TAB:
			if (v._shift) render_movie();
			else write_img();
			clear_img(v.img); break;
		
		case K_R: v._rerender = 1; v._R = !v._R;
			v.render_mode = (v.render_mode+1) % RENDERMODES_MAX; break;
		//^toggles
		case K_H: v._H = !v._H;
			if (v._shift) {v._debug = !v._debug;}
			else {v._help = !v._help;} break;
		case K_L: v.lookat_toggle = !v.lookat_toggle; break;
		case K_M: v.motion_enabled = !v.motion_enabled; break;

		case K_0: v._0 = 1; break;
		case K_1: v._1 = 1; break;
		case K_2: v._2 = 1; break;
		case K_3: v._3 = 1; break;
		case K_4: v._4 = 1; break;
		case K_5: v._5 = 1; break;
		case K_6: v._6 = 1; break;
		case K_7: v._7 = 1; break;
		case K_8: v._8 = 1; break;
		case K_9: v._9 = 1; break;

		case K_NP_0: v._np0 = 1; break;
		case K_NP_1: v._np1 = 1; break;
		case K_NP_2: v._np2 = 1; break;
		case K_NP_3: v._np3 = 1; break;
		case K_NP_4: v._np4 = 1; break;
		case K_NP_5: v._np5 = 1; break;
		case K_NP_6: v._np6 = 1; break;
		case K_NP_7: v._np7 = 1; break;
		case K_NP_8: v._np8 = 1; break;
		case K_NP_9: v._np9 = 1; break;

		case K_P: v._p = 1; break;

		//holds
		case K_UP: v._up = 1; break;
		case K_DOWN: v._down = 1; break;
		case K_LEFT: v._left = 1; break;
		case K_RIGHT: v._right = 1; break;
		/*[wasd]*/
		case K_SPACE: v._space = 1; break;
		case K_SHIFT: v._shift = 1; break;
		case K_CTRL: v._ctrl = 1; break;

		//--end--//

		case K_X: plane_switch(X, YZ); break;
		case K_Y: plane_switch(Y, XZ); break;
		case K_Z: plane_switch(Z, XY); break;

		default: printf("[DOWN] key <%d>\n", k); break;
	}
	return (0);
}

int	handle_key_release(int k)
{
	switch (k)
	{
		case K_UP: v._up = 0; break;
		case K_DOWN: v._down = 0; break;
		case K_LEFT: v._left = 0; break;
		case K_RIGHT: v._right = 0; break;
		/*[wasd]*/
		case K_SPACE: v._space = 0; break;
		case K_SHIFT: v._shift = 0; break;
		case K_CTRL: v._ctrl = 0; break;
		default: printf("[UP] key <%d>\n", k); break;
	}
	return (0);
	(void) k;
}

void	_reset_consumable_clicks(void)
{
	v._lclick = 0; v._llift = 0;
	v._rclick = 0; v._rlift = 0;
	v._0 = 0, v._1 = 0, v._2 = 0, v._3 = 0, v._4 = 0, v._5 = 0, v._6 = 0, v._7 = 0; //v._8 = 0;//, v._9 = 0;
	v._np0 = 0, v._np1 = 0, v._np2 = 0, v._np3 = 0, v._np4 = 0, v._np5 = 0, v._np6 = 0, v._np7 = 0, v._np8 = 0, v._np9 = 0;
	v._p = 0;
}
