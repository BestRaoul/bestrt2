/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

texture	solid_color(color c)
{
	texture	t;

	t = (texture){};
	t.color_value = c;
	t.value = solid_color_return ;
	return (t);
}

texture	checkerboard(PFPN scale, texture even, texture odd)
{
	texture	t;

	t = (texture){};
	t.color_value = WHITE;
	t.value = checkeboard_return ;
	t.inv_scale = scale;
	t.checker_0 = t_shallow_copy(&even);
	t.checker_1 = t_shallow_copy(&odd);
	return (t);
}

static bool	bmp_read_into_texture(const bmp_read *r, texture *t)
{
	color		*image;
	int			x;
	int			y;


	image = gc_malloc(sizeof(color) * r->widht * r->height);
	x = 0;
	while (x < r->widht)
	{
		y = 0;
		while (y < r->height)
		{
			image[x + y * r->widht] = rgb2color(r->pixels[y][x]);
			y++;
		}
		x++;
	}
	t->image_width = r->widht;
	t->image_height = r->height;
	t->image = image;
	return (True);
}

texture	from_bmp(const char *filename)
{
	texture		t;
	bmp_read	r;

	t = (texture){};
	r = (bmp_read){0, 0, 0};
	if (!read_bmp(filename, &r))
		return (solid_color(ERROR_CYAN));
	if (!bmp_read_into_texture(&r, &t))
		return (solid_color(ERROR_CYAN));
	t.color_value = ERROR_CYAN;
	t.value = image_return;
	return (t);
}

texture	from_xmp(const char *filename)
{
	texture		t;
	bmp_read	r;

	t = (texture){};
	r = (bmp_read){0, 0, 0};
	if (!read_xmp(filename, &r))
		return (solid_color(ERROR_CYAN));
	if (!bmp_read_into_texture(&r, &t))
		return (solid_color(ERROR_CYAN));
	t.color_value = ERROR_CYAN;
	t.value = image_return;
	return (t);
}

texture	from_png(const char *filename)
{
	texture		t;
	bmp_read	r;

	t = (texture){};
	r = (bmp_read){0, 0, 0};
	if (!read_png(filename, &r))
		return (solid_color(ERROR_CYAN));
	if (!bmp_read_into_texture(&r, &t))
		return (solid_color(ERROR_CYAN));
	t.color_value = ERROR_CYAN;
	t.value = image_return;
	return (t);
}

texture	from_func(color (*uv_func)(vec3))
{
	texture	t;

	t = (texture){};
	t.color_value = ERROR_CYAN;
	t.value = func_return ;
	t.uv_func = uv_func;
	return (t);
}
