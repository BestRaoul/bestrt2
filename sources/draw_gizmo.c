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

void	gizmo_dot(vec3 pos, color c)
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
				draw_inbounds(pos.x + x, pos.y + y, c);
			y++;
		}
		x++;
	}
}

void	draw_line(vec3 start, vec3 end, color c, int (*draw_func)(vec3 p,
			color c))
{
	vec3	move;
	vec3	step;
	vec3	ve;

	if (v_in_bounds(end) && !v_in_bounds(start))
		return (draw_line(end, start, c, draw_func));
	move = v_sub(end, start);
	if (fabs(move.x) >= fabs(move.y))
		step = v3(1 * signd(move.x), move.y / fabs(move.x), move.z
				/ fabs(move.x));
	else
		step = v3(move.x / fabs(move.y), 1 * signd(move.y), move.z
				/ fabs(move.y));
	ve = start;
	while (draw_func(ve, c) && (fabs(ve.x - end.x) > 1 || fabs(ve.y
				- end.y) > 1))
	{
		ve = v_add(ve, step);
	}
	draw_func(ve, c);
	draw_func(end, c);
}

void	gizmo_line(vec3 start, vec3 end, color c)
{
	draw_line(start, end, c, draw_v_inbounds);
}

void	heat_line(vec3 start, vec3 end, color c)
{
	draw_line(start, end, c, draw_v_inheat);
}

void	gizmo_drag(vec3 start_pos, vec3 current_pos, color c)
{
	scribe("O", v3(start_pos.x - 2, start_pos.y + 4), c);
	scribe("x", v3(current_pos.x - 2, current_pos.y + 4), c);
	draw_debug_line(start_pos, current_pos, WHITE);
}

// void	gizmo_nshape(int n, vec3 pos, vec3 rot, vec3 scale, PFPN r_offset,
// 		color c)
// {
// 	vec3	this;
// 	vec3	next;

// 	PFPN _a = 0 + r_offset; // 45'
// 	this = v3(scale.x * sin(_a), scale.y * cos(_a));
// 	this = rotate3(this, rot);
// 	for (int i = 1; i <= n; i++)
// 	{
// 		_a = MYPI * 2 / n * i + r_offset; // 45', 135', 225', 315', 45'
// 		next = v3(scale.x * sin(_a), scale.y * cos(_a));
// 		next = rotate3(next, rot);
// 		gizmo_line(v_add(pos, this), v_add(pos, next), c);
// 		this = next;
// 	}
// }
// void	gizmo_box(vec3 pos, int w, int h, color c)
// {
// 	for (int x = -w; x <= w; x++)
// 		for (int y = -h; y <= h; y++)
// 			draw_inbounds(pos.x + x, pos.y + y, c);
// 	(void)gizmo_box;
// }
