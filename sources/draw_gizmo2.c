/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draws.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static int	debug_pixel(vec3 p, color c)
{
	if (!in_bounds(p.x, p.y))
		return (0);
	mlx_pixel_put(v.mlx, v.win, p.x, p.y, color2rgb(c));
	return (1);
}

void	draw_debug_line(vec3 start, vec3 end, color c)
{
	draw_line(start, end, c, debug_pixel);
}

void	draw_debug_dot(vec3 pos, color c)
{
	int	x;
	int	y;

	x = -2;
	while (x <= 2)
	{
		y = -2;
		while (y <= 2)
		{
			if (!((x == -2 || x == 2) && (y == -2 || y == 2)))
				if (in_bounds(pos.x + x, pos.y + y))
					mlx_pixel_put(v.mlx, v.win, pos.x + x, pos.y + y,
						color2rgb(c));
			y++;
		}
		x++;
	}
}

// -- PROJECTED

// line onto screen limits
// void	draw_projected_line(vec3 start, vec3 end, vec3 c)
// {
// 	vec3	dir;

// 	start = world_to_screenpos(start);
// 	end = world_to_screenpos(end);
// 	dir = from_to(start, end);
// 	heat_line(start, end, c);
// }

void	draw_projected_dot(vec3 pos, color c)
{
	vec3	_p;
	int		x;
	int		y;

	pos = world_to_screenpos(pos);
	_p = v3(0, 0, pos.z);
	x = -2;
	while (x <= 2)
	{
		y = -2;
		while (y <= 2)
		{
			if ((x == -2 || x == 2) && (y == -2 || y == 2))
			{
				y++;
				continue ;
			}
			_p.x = (int)(pos.x + x);
			_p.y = (int)(pos.y + y);
			draw_v_inheat(_p, c);
			y++;
		}
		x++;
	}
}

// void	draw_projected_point(vec3 pos, color c)
// {
// 	pos = world_to_screenpos(pos);
// 	draw_v_inheat(pos, c);
// }

// // SEGV so much
void	draw_projected_line(vec3 start, vec3 end, vec3 c)
{
	start = world_to_screenpos(start);
	end = world_to_screenpos(end);
	if (start.z < 0 && end.z < 0)
		return ;
	if (end.z < 0)
		vswap(&start, &end);
	if (start.z > 0 && end.z > 0 && ((start.x <= 0 && end.x <= 0)
			|| (start.x >= v.w && end.x >= v.w) || (start.y <= 0 && end.y <= 0)
			|| (start.y >= v.h && end.y >= v.h)))
		return ;
	if (!v_in_bounds(start) || (start.z < 0 && !v.orthographic_toggle))
	{
		start = project_onto_screen_limits(start, v_scal(v_sub(start, end),
					signd(start.z)));
	}
	if (!v_in_bounds(end) || (end.z < 0 && !v.orthographic_toggle))
	{
		end = project_onto_screen_limits(end, v_scal(v_sub(end, start),
					signd(end.z)));
	}
	start = (vec3){(int)start.x, (int)start.y};
	end = (vec3){(int)end.x, (int)end.y};
	if (v_in_bounds(start) && v_in_bounds(end) && !v_eq(start, end))
		heat_line(start, end, c);
}
