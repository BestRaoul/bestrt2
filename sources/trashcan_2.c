/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	**alloc_2d(int size, int w, int h)
{
	void	*mem_start;
	void	**ptr_ptr;
	int		i;

	mem_start = gc_malloc(size * w * h);
	ptr_ptr = gc_malloc(sizeof(void *) * w);
	i = 0;
	while (i < w)
	{
		ptr_ptr[i] = (mem_start + i * h * size);
		i++;
	}
	return (ptr_ptr);
}

#ifdef LINUX

void	set_cursor(unsigned int xc)
{
	t_xvar		*xvar;
	t_win_list	*win;
	Cursor		cursor;

	xvar = (t_xvar *)v.mlx;
	win = (t_win_list *)v.win;
	xc %= 154;
	printf("xc: %u\n", xc);
	cursor = XCreateFontCursor(xvar->display, xc);
	XDefineCursor(xvar->display, win->window, cursor);
	XMoveWindow(xvar->display, win->window, 955, 0);
}
#else

void	set_cursor(unsigned int xc)
{
	static int	notified = 0;

	if (!notified)
		dprintf(2, "["ANSI_YELLOW"WARNING"ANSI_RESET"] "
			"Cursor modification disabled as not on LINUX\n");
	notified = 1;
	(void) xc;
}
#endif

color	invert_color(color c)
{
	return ((color){1. - c.x, 1. - c.y, 1. - c.z});
}

color	get_inverted(vec3 p)
{
	return (invert_color(rgb2color(get_pixel(p.x, p.y))));
}
