/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raster.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	draw_grid_and_cardinals(void)
{
	int		far;
	int		step;
	color	c;
	int		i;

	far = 1000;
	step = 15;
	i = -step;
	while (i <= step)
	{
		if (i == 0)
		{
			i++;
			continue ;
		}
		c = color_lerp(step * step - i * i, BLACK, WHITE);
		draw_projected_line(v3(-far, 0, i), v3(far, 0, i), c);
		draw_projected_line(v3(i, 0, -far), v3(i, 0, far), c);
		i++;
	}
	draw_projected_line(v3(-far), v3(far), new_color(1, .2, .2));
	draw_projected_line(v3(0, 0, -far), v3(0, 0, far), new_color(0, .4, 1));
}

// should draw a lamp gizmo not just a dot
void	draw_light_gizmo(t_light *l)
{
	if (l->is_dir)
	{
		draw_projected_dot(l->transform.pos, l->col);
		draw_projected_line(l->transform.pos, v_add(l->transform.pos, l->dir),
			l->col);
	}
	else
	{
		draw_projected_dot(l->transform.pos, l->col);
	}
}

void	__swap_yz(vec3 *v)
{
	*v = v3(v->x, v->z, v->y);
}

// void	draw_lookat(void)
// {
// 	vec3	at;
// 	PFPN	dist_to;
// 	PFPN	d;
//
// 	at = v.lookat;
// 	if (v.selected)
// 		at = v.selected->pos;
// 	dist_to = vec_dist(v.camera_pos, at);
// 	d = dist_to / 20;
// 	draw_projected_dot(at, v3(.8, .6, .1));
// 	draw_projected_line(v_add(at, v3(0, d, 0)), v_add(at, v3(0, -d, 0)), GREEN);
// 	draw_projected_line(v_add(at, v3(d, 0, 0)), v_add(at, v3(-d, 0, 0)), RED);
// 	draw_projected_line(v_add(at, v3(0, 0, d)), v_add(at, v3(0, 0, -d)), BLUE);
// }
