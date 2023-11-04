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
void	raster_lamps(void);
void	raster_selection(void);

void	draw_grid_and_cardinals(void)
{
	int far = 1000;
	int step = 15;
	for (int i=-step; i<=step; i++)
	{
		if (i==0) continue;
		color c = color_lerp(step*step - i*i, WHITE, WHITE);
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
	vec3 at;
	at = v.lookat;
	if (v.selected)
		at = v.selected->pos;
	
	double dist_to = vec_dist(v.camera_pos, at);
	double d = dist_to/20;
	color	line_color = WHITE;

	draw_projected_dot(at, v3(.8, .6, .1));
	draw_projected_line(v_add(at, v3(0,  d, 0)),
						v_add(at, v3(0, -d, 0)),
						GREEN);
	draw_projected_line(v_add(at, v3( d, 0, 0)),
						v_add(at, v3(-d, 0, 0)),
						RED);
	draw_projected_line(v_add(at, v3(0, 0,  d)),
						v_add(at, v3(0, 0, -d)),
						BLUE);

}

void	raster(void)
{
	if (v._rerender)
		reset_heatmap();
	
	draw_grid_and_cardinals();
	raster_items();
	raster_lamps();
	raster_selection();

	// draw_lookat();
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

void	draw_light_gizmo(t_light *l)
{
	if (l->is_dir)
	{
		draw_projected_dot(l->transform.pos, l->col);
		draw_projected_line(l->transform.pos, v_add(l->transform.pos, l->dir), l->col);
	}
	else
	{
		draw_projected_dot(l->transform.pos, l->col);
	}
}

void	raster_lamps(void)
{
	for (int i=0; i<v.light_count; i++)
		draw_light_gizmo(&v.lights[i]);
}

void	raster_selection(void)
{
	if (v.selected == NULL) return;

	vec3 spos = world_to_screenpos(v.selected->pos);


	ri r = get_rotation_indicator(v.selected->pos, spos);

	gizmo_dot(r.anchor, vrgb(246,161,38));
	

	return;

	static int rrot = 0; 
	rrot++;


	if (!v_in_bounds(spos)) return;

	gizmo_nshape(8, spos, v3(0, 0, rrot/360.0*MYPI), v3(14, 14), 0, new_color(.8, .4, 0));
	gizmo_dot(spos, new_color(1, .2, .2));
}
