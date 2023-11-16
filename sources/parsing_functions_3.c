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

int	parse_sphere(const char **line)
{
	t_item	new;

	new.transform = (tfm){v3(), v3(), v_3(1)};
	new.hit = hit_sphere;
	new.raster = raster_sphere;
	new.transform.pos = atov3(*(line++), v3());
	new.transform.rot = atov3(*(line++), v3());
	new.transform.scale = atov3(*(line++), v_3(1));
	new.mat = atomaterial(line, new_m(random_v3()));
	add_item_(new);
	return (1);
}

int	parse_plane(const char **line)
{
	t_item	new;
	vec3	normal;

	new.transform = (tfm){v3(), v3(), v_3(1)};
	new.hit = hit_plane;
	new.raster = raster_plane;
	new.transform.pos = atov3(*(line++), v3());
	new.transform.rot = atov3(*(line++), v3());
	new.transform.scale = atov3(*(line++), v_3(1));
	new.mat = atomaterial(line, new_m(random_v3()));
	add_item_(new);
	return (1);
}

int	parse_quad(const char **line)
{
	t_item	new;

	new.transform = (tfm){v3(), v3(), v_3(1)};
	new.hit = hit_quad;
	new.raster = raster_quad;
	new.transform.pos = atov3(*(line++), v3());
	new.transform.rot = atov3(*(line++), v3());
	new.transform.scale = atov3(*(line++), v_3(1));
	new.mat = atomaterial(line, new_m(random_v3()));
	add_item_(new);
	return (1);
}

int	parse_ss_quad(const char **line)
{
	t_item	new;

	new.transform = (tfm){v3(), v3(), v_3(1)};
	new.hit = hit_ss_quad;
	new.raster = raster_quad;
	new.transform.pos = atov3(*(line++), v3());
	new.transform.rot = atov3(*(line++), v3());
	new.transform.scale = atov3(*(line++), v_3(1));
	new.mat = atomaterial(line, new_m(random_v3()));
	add_item_(new);
	return (1);
}

int	parse_box(const char **line)
{
	t_item	new;

	new.transform = (tfm){v3(), v3(), v_3(1)};
	new.hit = hit_box;
	new.raster = raster_box;
	new.transform.pos = atov3(*(line++), v3());
	new.transform.rot = atov3(*(line++), v3());
	new.transform.scale = atov3(*(line++), v_3(1));
	new.mat = atomaterial(line, new_m(random_v3()));
	add_item_(new);
	return (1);
}
