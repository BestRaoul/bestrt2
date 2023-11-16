/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raster_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	raster_quad(t_item *item)
{
	vec3	c[4 + 1];
	vec3	normal;

	c[1] = parent_to(v3(1, 0, 1), &item->transform);
	c[2] = parent_to(v3(1, 0, -1), &item->transform);
	c[3] = parent_to(v3(-1, 0, -1), &item->transform);
	c[4] = parent_to(v3(-1, 0, 1), &item->transform);
	link_lines(&(c[1]), 4, item->mat.base_color.color_value);
	if (v._debug)
	{
		dot_them(&(c[1]), 4, invert_color(item->mat.base_color.color_value));
		normal = rotate3(v3(0, .2, 0), item->transform.rot);
		draw_projected_line(v_add(item->transform.pos, normal),
			item->transform.pos, item->mat.base_color.color_value);
		draw_projected_dot(v_add(item->transform.pos, normal),
			item->mat.base_color.color_value);
	}
}

void	raster_plane(t_item *item)
{
	vec3	c[4 + 1];
	vec3	normal;

	c[1] = parent_to(v3(1, 0, 1), &item->transform);
	c[2] = parent_to(v3(1, 0, -1), &item->transform);
	c[3] = parent_to(v3(-1, 0, -1), &item->transform);
	c[4] = parent_to(v3(-1, 0, 1), &item->transform);
	link_lines(&(c[1]), 4, item->mat.base_color.color_value);
	draw_projected_line(c[2], c[4], item->mat.base_color.color_value);
	draw_projected_line(c[3], c[1], item->mat.base_color.color_value);
	if (v._debug)
	{
		dot_them(&(c[1]), 4, invert_color(item->mat.base_color.color_value));
		normal = rotate3(v3(0, .2, 0), item->transform.rot);
		draw_projected_line(v_add(item->transform.pos, normal),
			item->transform.pos, item->mat.base_color.color_value);
		draw_projected_dot(v_add(item->transform.pos, normal),
			item->mat.base_color.color_value);
	}
}

void	link_lines(vec3 *points, int count, color c)
{
	int	i;

	i = 0;
	while (i < count - 1)
	{
		draw_projected_line(points[i], points[i + 1], c);
		i++;
	}
	draw_projected_line(points[count - 1], points[0], c);
}

void	dot_them(vec3 *points, int count, color c)
{
	int	i;

	i = 0;
	while (i < count)
	{
		draw_projected_dot(points[i], c);
		i++;
	}
}

// //SQUARE - 1
// void	raster_square(t_item *item)
// {
// 	vec3 c1, c2, c3, c4;
//
// 	c1 = parent_to(v3( 1,  1), &item->transform);
// 	c2 = parent_to(v3( 1, -1), &item->transform);
// 	c3 = parent_to(v3(-1,  1), &item->transform);
// 	c4 = parent_to(v3(-1, -1), &item->transform);
//
// 	gizmo_line(c1, c2, item->mat.base_color.color_value);
// 	gizmo_line(c2, c3, item->mat.base_color.color_value);
// 	gizmo_line(c3, c4, item->mat.base_color.color_value);
// 	gizmo_line(c4, c1, item->mat.base_color.color_value);
// }
// //CIRCLE - 2
// // void	raster_circle(t_item *item)
// // {
// // 	gizmo_nshape(max(item->transform.scale.x*2.0/50.0, 10),
// 		item->transform.pos, item->transform.rot, item->transform.scale, 0,
// 		item->mat.base_color.color_value);
// // // }
// // // //TRI.. 3,4,0
// // // void	raster_tri(t_item *item)
// // // {
// // // 	gizmo_nshape(3, item->transform.pos, item->transform.rot,
// 		item->transform.scale, MYPI, item->mat.base_color.color_value);
// // // }
// // // void	raster_hex(t_item *item)
// // // {
// // // 	gizmo_nshape(6, item->transform.pos, item->transform.rot,
// 		item->transform.scale, MYPI/6, item->mat.base_color.color_value);
// // // }
// // void	raster_line(t_item *item)
// // {
// // 	gizmo_line(item->transform.pos,
//
// v3(item->transform.pos.x+sin(item->transform.rot.x)*item->transform.scale.x,
//
// item->transform.pos.y+cos(item->transform.rot.x)*item->transform.scale.x),
// 				item->mat.base_color.color_value);
// }
