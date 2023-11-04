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

void	remove_lamp(t_light *l_ptr)
{
	t_light	*last;

	last = &(v.lights[v.light_count - 1]);
	memcpy(l_ptr, last, sizeof(t_light));
	*last = (t_light){};
	v.light_count--;
}

t_item	get_item_default(void)
{
	t_item	get;

	get = (t_item){(tfm){v3(0), v3(0), v3(1, 1, 1)}};
	get.mat = new_m(random_unit_vector());
	if (v._1)
	{
		get.raster = raster_sphere;
		get.hit = hit_sphere;
	}
	else if (v._2)
	{
		get.raster = raster_box;
		get.hit = hit_box;
	}
	else if (v._3)
	{
		get.raster = raster_cylinder;
		get.hit = hit_cylinder;
	}
	else if (v._4)
	{
		get.raster = raster_pyramid;
		get.hit = hit_cone;
	}
	return (get);
}

t_light	get_lamp_default(void)
{
	t_light	get;

	get = (t_light){};
	get.transform = (tfm){v3(0), v3(0), v3(1, 1, 1)};
	get.col = WHITE;
	get.dir = v3(0, -1, 0);
	get.intensity = 1.0;
	get.is_dir = False;
	if (v._5)
	{
		get.is_dir = False;
	}
	else if (v._6)
	{
		get.is_dir = True;
	}
	return (get);
}
