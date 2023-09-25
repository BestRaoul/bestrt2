/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	add_item(t_item t)
{
	t_item	*new_items = calloc(v.item_count + 1, sizeof(t_item));
	for (int i=0; i<v.item_count; i++)
		new_items[i] = v.items[i];
	new_items[v.item_count] = t;
	v.items = new_items;
	v.item_count++;
}

void	remove_item(t_item *t_ptr)
{
	t_item	*new_items = calloc(v.item_count - 1, sizeof(t_item));
	int j = 0;
	for (int i=0; i<v.item_count; i++)
		if (&(v.items[i]) != t_ptr)
			new_items[j++] = v.items[i];
	v.items = new_items;
	v.item_count--;
}

t_item	get_item_default()
{
	t_item	get = (t_item){v3(0),
						v3(1, 1, 1),
						v3(0),
						new_lambertian(c3(0,0,0)),
						NULL,  //*raster
						NULL}; //*hit
	if (v._1)
	{
		get.mat.base_color = c3(0, 1, 0);
		get.raster = raster_box, get.hit = hit_box;
	}
	else if (v._2)
	{
		get.mat.base_color = c3(1, 0, 0);
		get.raster = raster_sphere, get.hit = hit_sphere;
	}
	else if (v._3)
	{
		get.mat.base_color = c3(1, .8, .1);
		get.raster = raster_pyramid, get.hit = hit_pyramid;
	}
	else if (v._4)
	{
		get.mat.base_color = c3(.1, .5, 1);
		get.raster = raster_cylinder, get.hit = hit_cylinder;
	}
	else if (v._0)
	{
		get.mat.base_color = c3(1, 1, 1);
		get.raster = raster_line, get.hit = hit_line;
	}
	return (get);
}
