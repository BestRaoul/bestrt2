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

static void	repeat(PFPN *_u, PFPN *_v)
{
	*_u -= (int)*_u / 1;
	*_v -= (int)*_v / 1;
	if (*_u < 0)
		*_u = 1 - fabs(*_u);
	if (*_v < 0)
		*_v = 1 - fabs(*_v);
}

// void    clip(PFPN *_u, PFPN *_v)
// {
//     return ;
// }

// -- RETURNS

color	solid_color_return(PFPN _u, PFPN _v, const texture *self)
{
	return (self->color_value);
}

color	checkeboard_return(PFPN _u, PFPN _v, const texture *self)
{
	int	i_u;
	int	i_v;
	int	even_odd;

	repeat(&_u, &_v);
	_u /= self->inv_scale;
	_v /= self->inv_scale;
	i_u = (int)_u;
	i_v = (int)_v;
	even_odd = ((i_v % 2 + i_u % 2) && ((i_v + 1) % 2 + (i_u + 1) % 2));
	_u -= (int)_u / 1;
	_v -= (int)_v / 1;
	if (even_odd)
		return (evaluate(self->checker_0, _u, _v));
	else
		return (evaluate(self->checker_1, _u, _v));
}

color	image_return(PFPN _u, PFPN _v, const texture *self)
{
	int		x;
	int		y;
	color	pixel;

	repeat(&_u, &_v);
	if (self->image_height <= 0)
		return (ERROR_CYAN);
	_u = clamp_(_u);
	_v = 1.0 - clamp_(_v);
	x = _u * (self->image_width - 1);
	y = _v * (self->image_height - 1);
	pixel = self->image[x + y * self->image_width];
	return (pixel);
}

color	func_return(PFPN _u, PFPN _v, const texture *self)
{
	return (self->uv_func(v3(_u, _v)));
}
