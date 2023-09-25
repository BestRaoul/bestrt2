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

color   solid_color_return(double u, double v, const texture *self);
color   checkeboard_return(double u, double v, const texture *self);
color   image_return(double u, double v, const texture *self);

color   evaluate(texture *t, double u, double v)
{
    return t->value(u, v, t);
}

double   evaluate_bw(texture *t, double u, double v)
{
    color get = t->value(u, v, t);
    return (get.x + get.y + get.z) / 3.0;
}

texture *t_deep_copy(texture t)
{
    texture *copy_t = malloc(sizeof(texture));
    memcpy(copy_t, &t, sizeof(texture));
    return copy_t;
}

texture solid_color(color c)
{
    texture t;
    t.color_value = c;
    t.value = solid_color_return;

    return t;
}

texture checkerboard(double scale, texture even, texture odd)
{
    texture t;
    t.color_value = WHITE;
    t.value = checkeboard_return;
    t.inv_scale = scale;
    t.checker_0 = t_deep_copy(even);
    t.checker_1 = t_deep_copy(odd);
    return t;
}

texture from_bmp(const char *filename)
{
    texture t;

    // READ BMP HERE
    bmp_read r = (bmp_read){0, 0, 0};
    readBMP(filename, &r);
    color *image = malloc(sizeof(color) * r.widht * r.height);
    for (int x=0; x<r.widht; x++)
        for (int y=0; y<r.height; y++)
            image[x + y * r.widht] = rgb2color(r.pixels[y][x]);
    
    t.image_width = r.widht;
    t.image_height = r.height;
    t.image = image;
    t.value = image_return;

    return t;
}

// -- RETURNS

color   solid_color_return(double u, double v, const texture *self)
{
    return self->color_value;
}

color   checkeboard_return(double u, double v, const texture *self)
{
	if (u < 0) u = -u;
	if (v < 0) u = -v;
	//printf("B - u: %f and v: %f\n", u, v);
	u /= self->inv_scale;
	//0-1 / .1
	//0-10
	//
	v /= self->inv_scale;
	int _u = (int)u;
	int _v = (int)v;
	int even_odd = _v%2+_u%2 && (_v+1)%2+(_u+1)%2;
	u -= (int)u/1;
	v -= (int)v/1;
	return even_odd ?
		self->checker_1->value(u,v, self->checker_1) :
		self->checker_0->value(u,v, self->checker_0);
}

color   image_return(double u, double v, const texture *self)
{
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (self->image_height <= 0) return v3(0,1,1);

    // Clamp input texture coordinates to [0,1] x [1,0]
    interval oy = (interval){0, 1};
    u = clamp(oy, u);
    v = 1.0 - clamp(oy, v);  // Flip V to image coordinates

    int x = u * self->image_width;
    int y = v * self->image_height;
    color pixel = self->image[x + y * self->image_width];

    return pixel;
}
