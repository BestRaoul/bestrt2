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
	(void)x;
	(void)y;
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
	else if (!v._shift && (button == ZOOM_IN || button == ZOOM_OUT))
	{
		v._scroll += ((button == ZOOM_IN) - (button == ZOOM_OUT));
		set_cursor(v._scroll);
	}
	else if (v._shift && (button == ZOOM_IN || button == ZOOM_OUT))
	{
		v.focus_dist += 0.1 * ((button == ZOOM_IN) - (button == ZOOM_OUT));
		v._rerender = 1;
	}
	return (0);
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
	(void)button;
	(void)x;
	(void)y;
}

void	plane_switch(enum e_plne _plane, enum e_plne _excluded)
{
	if (!v._shift)
		v.plane = _plane * (v.plane != _plane);
	else
		v.plane = _excluded * (v.plane != _excluded);
}
