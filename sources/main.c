/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

t_vars	v = {};

void	vars_init(void)
{
	v.w = WIDTH * UPSCALE;
	v.h = HEIGHT * UPSCALE;
	v.mlx = mlx_init();
	if (v.mlx == NULL)
		exit(1);
	v.win = mlx_new_window(v.mlx, v.w, v.h, "bruh");
	v.img.mlx_img = mlx_new_image(v.mlx, v.w, v.h);
	v.img.addr = mlx_get_data_addr(v.img.mlx_img, &v.img.bpp,
			&v.img.line_len, &v.img.endian);

	//DATA
	gettimeofday(&v.last_update, 0);

	v.item_count = 0;
	v.items = NULL;

	v.motion_count = 0;
	v.motions = NULL;

	init_scene();
}

int	my_exit(void) {exit(0); return (0);}

void	hooks(void)
{
	mlx_loop_hook(v.mlx, loop, NULL);
	mlx_hook(v.win, 2, 1L << 0, handle_key_press, NULL);
	mlx_hook(v.win, 3, 1L << 1, handle_key_release, NULL);
	mlx_hook(v.win, BUTTONPRESS, 1L << BUTTONPRESSMASK, handle_mouse_press, NULL);
	mlx_hook(v.win, BUTTONRELEASE, 1L << BUTTONRELEASEMASK,
		handle_mouse_release, NULL);
	mlx_hook(v.win, MOTIONNOTIFY, 1L << POINTERMOTIONMASK,
		handle_mouse_move, NULL);
	mlx_hook(v.win, 17, 0, my_exit, NULL);
}

void	set_cursor(t_xvar *xvar, t_win_list *win, unsigned int xc)
{
	xc %= 154;

	printf("xc: %u\n",xc);

	Cursor cursor = XCreateFontCursor(xvar->display, xc);
	XDefineCursor(xvar->display, win->window, cursor);
	
	XMoveWindow(xvar->display, win->window, 955, 0);

	(void) cursor;
}

int	main(void)
{
	vars_init();
	
	srand (time ( NULL));

	set_cursor(v.mlx, v.win, XC_crosshair);
	hooks();
	mlx_loop(v.mlx);
	return (0);
}

