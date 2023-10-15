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
color   func_return(double _u, double _v, const texture *self);



color getGaussianBlur(int x, int y, const texture *self) {
    float kernel[5][5] = {
        {1.0f, 4.0f,  6.0f,  4.0f, 1.0f},
        {4.0f, 16.0f, 24.0f, 16.0f, 4.0f},
        {6.0f, 24.0f, 36.0f, 24.0f, 6.0f},
        {4.0f, 16.0f, 24.0f, 16.0f, 4.0f},
        {1.0f, 4.0f,  6.0f,  4.0f, 1.0f}
    };

    color sum = v3(0.0);
    int kCenter = 2; // Center of the kernel

    double width = self->image_width;
    double height = self->image_height;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int xOffset = x + i*3 - kCenter;
            int yOffset = y + j*3 - kCenter;

            // Check bounds
            if (xOffset >= 0 && xOffset < width && yOffset >= 0 && yOffset < height) {
                color pixelValue = self->value(xOffset / width, yOffset / height, self);
                sum.x += kernel[i][j] * pixelValue.x;
                sum.y += kernel[i][j] * pixelValue.y;
                sum.z += kernel[i][j] * pixelValue.z;
            }
        }
    }

    // Normalize the result
    return v_scal(sum, 1.0 / 256.0f);
}
//9 points evaluation
color   evaluate_spread(texture *t, double _u, double _v, double _spread)
{
    int s=2;
    color lo = v3();
    for (int i=-s; i<=s; i++)
        for (int j=-s; j<=s; j++)
            lo = v_add(lo, t->value(_u + (i * _spread), _v + (j * _spread), t));
    return v_scal(lo, 1.0 / (s*s));
}

color   evaluate(texture *t, double _u, double _v)
{
    return t->value(_u, _v, t);
}

double   evaluate_bw(texture *t, double _u, double _v)
{
    color get = t->value(_u, _v, t);
    return (get.x + get.y + get.z) / 3.0;
}

// --HELPRES

texture *t_shallow_copy(texture *t)
{
    texture *copy_t = malloc(sizeof(texture));
    memcpy(copy_t, t, sizeof(texture));
    return copy_t;
}

texture *t_deep_copy(texture *t)
{
    texture *copy_t = malloc(sizeof(texture));
    memcpy(copy_t, t, sizeof(texture));
    copy_t->image = malloc(sizeof(color)*t->image_height*t->image_width);
    memcpy(copy_t->image, t->image, sizeof(color)*t->image_height*t->image_width);
    return copy_t;
}

// -- TEXTURES

texture solid_color(color c)
{
    texture t = (texture){};
    t.color_value = c;
    t.value = solid_color_return;

    return t;
}

texture checkerboard(double scale, texture even, texture odd)
{
    texture t = (texture){};
    t.color_value = WHITE;
    t.value = checkeboard_return;
    t.inv_scale = scale;
    t.checker_0 = t_shallow_copy(&even);
    t.checker_1 = t_shallow_copy(&odd);
    return t;
}

texture from_bmp(const char *filename)
{
    texture t = (texture){};
    t.color_value = ERROR_CYAN;

    // READ BMP HERE
    bmp_read r = (bmp_read){0, 0, 0};
    if (!readBMP(filename, &r))
    {
        r.widht = 1;
        r.height = 1;
        int *x = malloc(sizeof(int));
        int **x_ptr = malloc(sizeof(int *));
        *x = color2rgb(ERROR_CYAN);
        *x_ptr = x;
        r.pixels = x_ptr;
    }
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

texture from_func(color (*uv_func)(vec3))
{
    texture t = (texture){};
    t.color_value = ERROR_CYAN;
    t.value = func_return;
    t.uv_func = uv_func;

    return t;
}

// -- RETURNS

color   solid_color_return(double _u, double _v, const texture *self)
{
    return self->color_value;
}

color   checkeboard_return(double _u, double _v, const texture *self)
{
	if (_u < 0) _u = -_u;
	if (_v < 0) _v = -_v;
	//printf("B - u: %f and v: %f\n", u, v);
	_u /= self->inv_scale;
	//0-1 / .1
	//0-10
	//
	_v /= self->inv_scale;
	int i_u = (int)_u;
	int i_v = (int)_v;
	int even_odd = i_v%2+i_u%2 && (i_v+1)%2+(i_u+1)%2;
	_u -= (int)_u/1;
	_v -= (int)_v/1;
	return even_odd ?
		self->checker_1->value(_u,_v, self->checker_1) :
		self->checker_0->value(_u,_v, self->checker_0);
}

//ADD repeat, mirror patterns
color   image_return(double _u, double _v, const texture *self)
{
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (self->image_height <= 0) return v3(0,1,1);

    // Clamp input texture coordinates to [0,1] x [1,0]
    _u = clamp_(_u);
    _v = 1.0 - clamp_(_v);  // Flip V to image coordinates

    int x = _u * (self->image_width - 1);
    int y = _v * (self->image_height - 1);
    color pixel = self->image[x + y * self->image_width];

    return pixel;
}

color   func_return(double _u, double _v, const texture *self)
{
    return self->uv_func(v3(_u, _v));
}
