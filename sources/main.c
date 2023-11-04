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

void	vars_init(int ac, char **av)
{
	texture	*b;
	int		xy;

	if (ac < 2)
		init_scene(100);
	else
		init_scene(ft_atoi(av[1]));
	b = t_deep_copy(&v.irradiance_map);
	xy = -1;
	while (++xy < b->image_height * b->image_width)
		b->image[xy] = getGaussianBlur(xy % b->image_width,
				xy / b->image_width, b);
	v.blurry_irradiance_map = *b;
	v.mlx = mlx_init();
	if (v.mlx == NULL)
		exit(1);
	v.win = mlx_new_window(v.mlx, v.w, v.h, "bruh");
	v.img.mlx_img = mlx_new_image(v.mlx, v.w, v.h);
	v.img.addr = mlx_get_data_addr(v.img.mlx_img, &v.img.bpp, &v.img.line_len,
			&v.img.endian);
	v.dist_heatmap = (double **)alloc_2d(sizeof(double), v.w, v.h);
	v.accumulate_img = (vec3 **)alloc_2d(sizeof(vec3), v.w, v.h);
	gettimeofday(&v.last_update, 0);
}

int	my_exit(void)
{
	exit(0);
	return (0);
}

void	hooks(void)
{
	mlx_loop_hook(v.mlx, loop, NULL);
	mlx_hook(v.win, 2, 1L << 0, handle_key_press, NULL);
	mlx_hook(v.win, 3, 1L << 1, handle_key_release, NULL);
	mlx_hook(v.win, BUTTONPRESS, 1L << BUTTONPRESSMASK, handle_mouse_press,
		NULL);
	mlx_hook(v.win, BUTTONRELEASE, 1L << BUTTONRELEASEMASK,
		handle_mouse_release, NULL);
	mlx_hook(v.win, MOTIONNOTIFY, 1L << POINTERMOTIONMASK, handle_mouse_move,
		NULL);
	mlx_hook(v.win, 17, 0, my_exit, NULL);
}

int	main(int ac, char **av)
{
	vars_init(ac, av);
	srand(time(NULL));
	set_cursor(XC_crosshair);
	hooks();
	mlx_loop(v.mlx);
	return (0);
}
