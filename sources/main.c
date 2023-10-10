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
	init_scene();
	texture *b = t_deep_copy(&v.irradiance_map);
	for (double y=0; y<b->image_height; y++)
		for (double x=0; x<b->image_width; x++)
			b->image[(int)x + (int)y * b->image_width] = getGaussianBlur(x, y, b);
	v.blurry_irradiance_map = *b;

	v.mlx = mlx_init();
	if (v.mlx == NULL)
		exit(1);
	v.win = mlx_new_window(v.mlx, v.w, v.h, "bruh");
	v.img.mlx_img = mlx_new_image(v.mlx, v.w, v.h);
	v.img.addr = mlx_get_data_addr(v.img.mlx_img, &v.img.bpp,
			&v.img.line_len, &v.img.endian);
	double *heatmap = malloc(sizeof(double) * v.w * v.h);
	v.dist_heatmap = malloc(sizeof(double *) * v.w);
	for (int i=0; i<v.w; i++)
		v.dist_heatmap[i] = &(heatmap[i * v.h]);

	vec3 *accumlates = malloc(sizeof(vec3) * v.w * v.h);
	v.accumulate_img = malloc(sizeof(vec3 *) * v.w);
	for (int i=0; i<v.w; i++)
		v.accumulate_img[i] = &(accumlates[i * v.h]);

	//DATA
	gettimeofday(&v.last_update, 0);
}

int	my_exit(void) {exit(0); return (0);}

int handle_window_resize(XConfigureEvent xce)
{
	if (xce.width != v.w)
	{
		write(1, "bruh", 4);
	}
	write(1, "x", 1);
	return 0;
}

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

	mlx_hook(v.win, ConfigureNotify, 0, handle_window_resize, NULL);
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

