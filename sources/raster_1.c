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

// draw_grid_and_cardinals();
void	raster(void)
{
	if (v._rerender)
		reset_heatmap();
	if (v._debug)
		draw_grid_and_cardinals();
	raster_items();
	raster_lamps();
	raster_selection();
}

void	raster_items(void)
{
	t_item	t;
	int		i;

	i = 0;
	while (i < v.item_count)
	{
		t = v.items[i];
		if (v_len(t.mat.base_color.color_value) != 0)
			t.mat.base_color.color_value = v_norm(t.mat.base_color.color_value);
		else
			t.mat.base_color.color_value = random_unit_vector();
		t.raster(&t);
		i++;
	}
}

void	raster_lamps(void)
{
	int	i;

	i = 0;
	while (i < v.light_count)
		draw_light_gizmo(&v.lights[i++]);
}

void	raster_selection(void)
{
	vec3	spos;
	ri		r;

	if (v.selected == NULL)
		return ;
	spos = world_to_screenpos(v.selected->pos);
	r = get_rotation_indicator(v.selected->pos, spos);
	gizmo_dot(r.anchor, vrgb(246, 161, 38));
	return ;
}
