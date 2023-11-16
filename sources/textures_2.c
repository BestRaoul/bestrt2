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

color	solid_color_return(PFPN u, PFPN v, const texture *self);
color	checkeboard_return(PFPN u, PFPN v, const texture *self);
color	image_return(PFPN u, PFPN v, const texture *self);
color	func_return(PFPN _u, PFPN _v, const texture *self);

texture	*t_shallow_copy(texture *t)
{
	texture	*copy_t;

	copy_t = gc_malloc(sizeof(texture));
	memcpy(copy_t, t, sizeof(texture));
	return (copy_t);
}

texture	*t_deep_copy(texture *t)
{
	texture	*copy_t;

	copy_t = gc_malloc(sizeof(texture));
	memcpy(copy_t, t, sizeof(texture));
	copy_t->image = gc_malloc(sizeof(color) * t->image_height * t->image_width);
	memcpy(copy_t->image, t->image, sizeof(color) * t->image_height
		* t->image_width);
	return (copy_t);
}

color	evaluate(texture *t, PFPN _u, PFPN _v)
{
	return (t->value(_u, _v, t));
}

PFPN	evaluate_bw(texture *t, PFPN _u, PFPN _v)
{
	color	get;

	get = t->value(_u, _v, t);
	return ((get.x + get.y + get.z) / 3.0);
}

// 9 points evaluation
color	evaluate_spread(texture *t, PFPN _u, PFPN _v, PFPN _spread)
{
	int		s;
	color	lo;
	int		i;
	int		j;

	s = 2;
	lo = v3();
	i = -s;
	while (i <= s)
	{
		j = -s;
		while (j <= s)
		{
			lo = v_add(lo, t->value(_u + (i * _spread), _v + (j * _spread), t));
			j++;
		}
		i++;
	}
	return (v_scal(lo, 1.0 / (s * s)));
}
