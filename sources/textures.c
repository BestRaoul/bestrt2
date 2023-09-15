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

texture *t_deep_copy(texture t)
{
    texture *copy_t = malloc(sizeof(texture));
    copy_t->color_value = t.color_value;
    copy_t->value = t.value;
    copy_t->inv_scale = t.inv_scale;
    copy_t->checker_0 = t.checker_0;
    copy_t->checker_1 = t.checker_1;
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
    //printf("u: %f and v: %f\n", u, v);
    return even_odd ?
        self->checker_1->value(u,v, self->checker_0) :
        self->checker_0->value(u,v, self->checker_1);
}
