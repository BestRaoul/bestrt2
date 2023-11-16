/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

#define FT_TRANSPARENCY 0

int	new_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

int	new_rgb(int r, int g, int b)
{
	return (new_trgb(FT_TRANSPARENCY, r, g, b));
}

color	new_color(PFPN r, PFPN g, PFPN b)
{
	return ((color){r, g, b});
}

color	color_lerp(PFPN t, color color1, color color2)
{
	PFPN	r;
	PFPN	g;
	PFPN	b;

	r = (1 - t) * color1.x + t * color2.x;
	g = (1 - t) * color1.y + t * color2.y;
	b = (1 - t) * color1.z + t * color2.z;
	return (new_color(r, g, b));
}

int	color2rgb(color c)
{
	static const interval	intensity = (interval){0.000, 0.999};

	return (new_rgb(clamp(intensity, c.x) * 256, clamp(intensity, c.y) * 256,
			clamp(intensity, c.z) * 256));
}
