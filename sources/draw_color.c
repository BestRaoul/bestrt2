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

int	get_r(int trgb);
int	get_g(int trgb);
int	get_b(int trgb);


int		new_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

int		new_rgb(int r, int g, int b)
{
	return new_trgb(FT_TRANSPARENCY, r, g, b);
}

color	new_color(double r, double g, double b)
{
	return (color){r, g, b};
}

color	color_lerp(double t, color color1, color color2)
{
	double r = (1-t)*color1.x + t*color2.x;
	double g = (1-t)*color1.y + t*color2.y;
	double b = (1-t)*color1.z + t*color2.z;
	return new_color(r, g, b);
}

int		color2rgb(color c)
{
	static const interval intensity = (interval){0.000, 0.999};

	return new_rgb(clamp(intensity, c.x) * 256,
				   clamp(intensity, c.y) * 256,
				   clamp(intensity, c.z) * 256);
}

color	rgb2color(int rgb)
{
	return new_color(get_r(rgb)/255.0,
					get_g(rgb)/255.0, 
					get_b(rgb)/255.0);
}

int	get_r(int trgb)
{
	return ((trgb >> 16) & 0xFF);
}

int	get_g(int trgb)
{
	return ((trgb >> 8) & 0xFF);
}

int	get_b(int trgb)
{
	return ((trgb >> 0) & 0xFF);
}