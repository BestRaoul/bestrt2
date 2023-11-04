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

t_item	*add_item(vec3 position, vec3 scale, vec3 rotation, material m,
		void (*raster)(t_item *), Bool (*hit)(const ray *, const interval,
			hit_record *, const t_item *))
{
	return (add_item_((t_item){(tfm){position, rotation, scale}, m, raster,
		hit}));
}

t_item	*add_item_(t_item t)
{
	if (v.item_count >= MAX_ITEM_COUNT)
	{
		printf("Too many items >%d, recompile with greater MAX_ITEM_COUNT\n",
			MAX_ITEM_COUNT);
		return (NULL);
	}
	v.items[v.item_count] = t;
	return (&(v.items[v.item_count++]));
}

void	remove_item(t_item *t_ptr)
{
	t_item	*last;

	last = &(v.items[v.item_count - 1]);
	memcpy(t_ptr, last, sizeof(t_item));
	*last = (t_item){};
	v.item_count--;
}

t_light	*add_lamp(color col, vec3 pos_dir, double intensity, Bool is_dir)
{
	if (is_dir)
		return (add_lamp_((t_light){(tfm){v3(), v3(), v3(1, 1, 1)}, col,
			pos_dir, intensity, True}));
	else
		return (add_lamp_((t_light){(tfm){pos_dir, v3(), v3(1, 1, 1)}, col,
			pos_dir, intensity, True}));
}

t_light	*add_lamp_(t_light l)
{
	if (v.light_count >= MAX_LIGHT_COUNT)
	{
		printf("Too many lamps >%d, recompile with greater MAX_LIGHT_COUNT\n",
			MAX_LIGHT_COUNT);
		return (NULL);
	}
	v.lights[v.light_count] = l;
	return (&(v.lights[v.light_count++]));
}
