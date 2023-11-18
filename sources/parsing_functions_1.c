/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upate_item.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/*
A 0.2 255,255,255		ambient light
C -50,0,20 0,0,0 70		camera
L -40,0,30 0.7 255,255,255	lamp
pl 0,0,0 0,1.0,0 255,0,225	PLANE
sp 0,0,20 20 255,0,0		sphere
cy 50.0,0.0,20.6 0,0,1.0 14.2 21.42 10,0,255 clinder
*/

int	parse_ibl(const char **line)
{
	v.irradiance_map = atotexture(line[0], WHITE_MAP);
	v.use_ibl = True;
	v.use_background = False;
	return (1);
}

int	parse_bg(const char **line)
{
	v.background = atotexture(line[0], WHITE_MAP);
	v.use_ibl = False;
	v.use_background = True;
	return (1);
}

int	parse_ambient_light(const char **line)
{
	v.ambient = atod(line[0], 0.0);
	v.ambient_color = atov3(line[1], WHITE);
	return (1);
}

int	parse_point_lamp(const char **line)
{
	t_light	new;

	new = (t_light){(tfm){v3(), v3(), v3()}, WHITE, v3(), 1.0, False};
	new.transform.pos = atov3(*(line++), v3(0, 0, 0));
	new.intensity = atod(*(line++), 1.0);
	new.col = atov3(*(line++), WHITE);
	new.is_dir = False;
	add_lamp_(new);
	return (1);
}

int	parse_directional_lamp(const char **line)
{
	t_light	new;

	new = (t_light){(tfm){v3(), v3(), v3()}, WHITE, v3(), 1.0, True};
	new.dir = atov3(*(line++), v3(0, -1, 0));
	new.intensity = atod(*(line++), 1.0);
	new.col = atov3(*(line++), WHITE);
	new.is_dir = True;
	print_pos(new.dir, "b");
	add_lamp_(new);
	print_pos(new.transform.rot, "a");
	return (1);
}
