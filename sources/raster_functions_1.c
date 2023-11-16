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

#define SPHERE_H_SPLITS 16
#define SPHERE_V_SPLITS 8

static void	raster_sphere_split(vec3 ring_scale, vec3 *ring,
	vec3 *previous_ring, t_item *item)
{
	vec3	new_scale;
	int		k;

	k = -1;
	while (++k <= SPHERE_H_SPLITS)
	{
		ring[k].z = 1;
		ring[k] = swap_yz(ring[k]);
	}
	new_scale = v3(item->transform.scale.x * ring_scale.x,
			item->transform.scale.y * ring_scale.y,
			item->transform.scale.z * ring_scale.x);
	k = -1;
	while (++k <= SPHERE_H_SPLITS)
		ring[k] = parent_to_virtual(ring[k],
				item->transform.pos, item->transform.rot, new_scale);
	link_lines(&(ring[0]), SPHERE_H_SPLITS,
		item->mat.base_color.color_value);
	k = -1;
	while (!v_eq(ring_scale, v3(sin(MYPI / SPHERE_V_SPLITS),
				cos(MYPI / SPHERE_V_SPLITS))) && ++k <= SPHERE_H_SPLITS)
		draw_projected_line(ring[k], previous_ring[k],
			item->mat.base_color.color_value);
}

// h_splits = segments	-min: 3
// v_splits = rings		-min: 3
void	raster_sphere(t_item *item)
{
	vec3	north_pole;
	vec3	south_pole;
	vec3	ring_scale[2 * SPHERE_V_SPLITS + 1];
	vec3	rings[SPHERE_V_SPLITS][SPHERE_H_SPLITS + 1];
	int		j;

	north_pole = parent_to(v3(0, 1, 0), &item->transform);
	south_pole = parent_to(v3(0, -1, 0), &item->transform);
	set_npoints(ring_scale, 2 * SPHERE_V_SPLITS, 0);
	j = 0;
	while (++j < SPHERE_V_SPLITS)
	{
		set_npoints(rings[j - 1], SPHERE_H_SPLITS, 0);
		raster_sphere_split(ring_scale[j], rings[j - 1], rings[j - 2], item);
	}
	j = -1;
	while (++j < SPHERE_H_SPLITS)
	{
		(draw_projected_line(rings[0][j], north_pole, item->mat.base_color
			.color_value), draw_projected_line(rings[SPHERE_V_SPLITS - 2][j],
			south_pole, item->mat.base_color.color_value));
	}
	if (v._debug)
		dot_them(rings[0], SPHERE_V_SPLITS * SPHERE_H_SPLITS,
			invert_color(item->mat.base_color.color_value));
}

#define CYLINDER_V_SPLITS 16
//
void	raster_cylinder(t_item *item)
{
	vec3	*l1;
	vec3	*l2;
	int		j;

	l1 = get_npoints(CYLINDER_V_SPLITS, 0);
	l2 = get_npoints(CYLINDER_V_SPLITS, 0);
	j = -1;
	while (++j < CYLINDER_V_SPLITS)
	{
		(__swap_yz(&l1[j]), __swap_yz(&l2[j]));
		l1[j] = v_add(l1[j], v3(0, 1, 0));
		l2[j] = v_add(l2[j], v3(0, -1, 0));
		l1[j] = parent_to(l1[j], &item->transform);
		l2[j] = parent_to(l2[j], &item->transform);
	}
	link_lines(l1, CYLINDER_V_SPLITS, item->mat.base_color.color_value);
	link_lines(l2, CYLINDER_V_SPLITS, item->mat.base_color.color_value);
	j = -1;
	while (++j < CYLINDER_V_SPLITS)
		draw_projected_line(l1[j], l2[j], item->mat.base_color.color_value);
	if (v._debug)
		(dot_them(l1, CYLINDER_V_SPLITS, invert_color(item->mat.base_color
					.color_value)), dot_them(l2, CYLINDER_V_SPLITS,
				invert_color(item->mat.base_color.color_value)));
	(gc_free(l1), gc_free(l2));
}

void	raster_box(t_item *item)
{
	vec3	c[8 + 1];

	c[1] = parent_to(v3(1, 1, 1), &item->transform);
	c[2] = parent_to(v3(1, 1, -1), &item->transform);
	c[3] = parent_to(v3(1, -1, -1), &item->transform);
	c[4] = parent_to(v3(1, -1, 1), &item->transform);
	c[5] = parent_to(v3(-1, 1, 1), &item->transform);
	c[6] = parent_to(v3(-1, 1, -1), &item->transform);
	c[7] = parent_to(v3(-1, -1, -1), &item->transform);
	c[8] = parent_to(v3(-1, -1, 1), &item->transform);
	link_lines(&(c[1]), 4, item->mat.base_color.color_value);
	link_lines(&(c[5]), 4, item->mat.base_color.color_value);
	draw_projected_line(c[1], c[5], item->mat.base_color.color_value);
	draw_projected_line(c[2], c[6], item->mat.base_color.color_value);
	draw_projected_line(c[3], c[7], item->mat.base_color.color_value);
	draw_projected_line(c[4], c[8], item->mat.base_color.color_value);
	if (v._debug)
	{
		dot_them(&(c[1]), 8, invert_color(item->mat.base_color.color_value));
	}
}

void	raster_pyramid(t_item *item)
{
	vec3	c[4 + 1];

	c[1] = v3(0, 1, 0);
	c[2] = v3(sin(0), -1, cos(0));
	c[3] = v3(sin(MYPI * 2 / 3), -1, cos(MYPI * 2 / 3));
	c[4] = v3(sin(MYPI * 4 / 3), -1, cos(MYPI * 4 / 3));
	c[1] = parent_to(c[1], &item->transform);
	c[2] = parent_to(c[2], &item->transform);
	c[3] = parent_to(c[3], &item->transform);
	c[4] = parent_to(c[4], &item->transform);
	draw_projected_line(c[1], c[2], item->mat.base_color.color_value);
	draw_projected_line(c[1], c[3], item->mat.base_color.color_value);
	draw_projected_line(c[1], c[4], item->mat.base_color.color_value);
	draw_projected_line(c[2], c[3], item->mat.base_color.color_value);
	draw_projected_line(c[3], c[4], item->mat.base_color.color_value);
	draw_projected_line(c[4], c[2], item->mat.base_color.color_value);
	if (v._debug)
	{
		dot_them(&(c[1]), 4, invert_color(item->mat.base_color.color_value));
	}
}
