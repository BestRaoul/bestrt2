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

void	raster_items(void);
void	raster_selection(void);

void	draw_grid_and_cardinals(void)
{
	int far = 1000;
	int step = 15;
	for (int i=-step; i<=step; i++)
	{
		if (i==0) continue;
		color c = color_lerp(step*step - i*i, WHITE, BLACK);
		draw_projected_line(v3(-far, 0, i), v3(far, 0, i), c);
		draw_projected_line(v3(i, 0, -far), v3(i, 0, far), c);
	}

	draw_projected_line(v3(-far), v3(far), new_color(1, .2, .2));
	draw_projected_line(v3(0,0, -far), v3(0, 0, far), new_color(0, .4, 1));
}

//might have error same as clear_img
void	reset_heatmap(void)
{
	memset(v.dist_heatmap[0], -1, sizeof(double) * v.w * v.h);
}

void	draw_lookat(void)
{
	double dist_to = vec_dist(v.camera_pos, v.lookat);
	double d = dist_to/25;
	color	line_color = WHITE;

	draw_projected_dot(v.lookat, v3(.8, .6, .1));
	draw_projected_line(v_add(v.lookat, v3(0,  d, 0)),
						v_add(v.lookat, v3(0, -d, 0)),
						GREEN);
	draw_projected_line(v_add(v.lookat, v3( d, 0, 0)),
						v_add(v.lookat, v3(-d, 0, 0)),
						RED);
	draw_projected_line(v_add(v.lookat, v3(0, 0,  d)),
						v_add(v.lookat, v3(0, 0, -d)),
						BLUE);

}

void	raster(void)
{
	v._rerender = 0;
	clear_img(v.img);
	reset_heatmap();

// /	draw_grid_and_cardinals();
	raster_items();
	raster_selection();

	draw_lookat();
}

void	raster_items(void)
{
	for (int i=0; i < v.item_count; i++)
	{
		t_item t = v.items[i];
		if (v_len(t.mat.base_color.color_value) != 0)
			t.mat.base_color.color_value = v_norm(t.mat.base_color.color_value);
		else
			t.mat.base_color.color_value = random_unit_vector();
		t.raster(&t);
	}
}

void	raster_selection(void)
{
	if (v.selection_mode == NONE) return;

	static int rrot = 0; 
	rrot++;

	vec3 spot = world_to_screenpos(v.selection_pos);

	if (!v_in_bounds(spot)) return;

	gizmo_nshape(6, spot, v3(0, 0, rrot/360.0*MYPI), v3(14, 14), 0, new_color(.8, .4, 0));
	gizmo_dot(spot, new_color(1, .2, .2));
	if (v.selection_mode == SCALE || v.selection_mode == ROTATE)
		gizmo_line(spot, v.mouse_pos, new_color(.8, .4, 0));
}
