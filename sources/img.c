/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	draw_raw(int x, int y, color c)
{
	char	*pixel;

	pixel = v.img.addr + v.img.line_len * y + x * (v.img.bpp / 8);
	*(unsigned int *)pixel = color2rgb(c);
}

int	get_pixel(int x, int y)
{
	char	*pixel;

	if (!in_bounds(x, y))
		return (color2rgb(ERROR_CYAN));
	pixel = v.img.addr + v.img.line_len * y + x * (v.img.bpp / 8);
	return (*(unsigned int *)pixel);
}

// 0 if not inbouds, 1 on success
int	draw_inbounds(int x, int y, color c)
{
	if (!in_bounds(x, y))
		return (0);
	draw_raw(x, y, c);
	return (1);
}

int	draw_v_inbounds(vec3 p, color c)
{
	return (draw_inbounds(p.x, p.y, c));
}

// 0 if not inbouds, 1 on success
int	draw_v_inheat(vec3 p, color c)
{
	if (!v_in_bounds(p))
		return (0);
	if (!check_heat(p))
	{
		set_heat(p);
		draw_raw(p.x, p.y, c);
	}
	return (1);
}
