/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   materials.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

material	new_lambertian(texture t)
{
	material	m;

	m = default_material();
	m.base_color = t;
	return (m);
}

material	new_lambertian_bump(texture t, texture bump)
{
	material	m;

	m = default_material();
	m.base_color = t;
	m.normal = bump;
	return (m);
}

material	new_metal(texture t, double roughness)
{
	material	m;

	m = default_material();
	m.base_color = t;
	m.metalic = BW_MAP(1.0);
	m.roughness = BW_MAP(roughness);
	NOT_IMPLEMENTED("textured roughness");
	return (m);
}

material	new_dielectric(texture t, double ior)
{
	material	m;

	m = default_material();
	m.base_color = t;
	m.ior = ior;
	m.transmission = BW_MAP(.8);
	NOT_IMPLEMENTED("dielectric custom transmission values");
	m.transmission_roughness = NO_MAP;
	NOT_IMPLEMENTED("transmission roughness map");
	return (m);
}

material	new_light(texture t, double emission_strength)
{
	material	m;

	m = default_material();
	m.base_color = NO_MAP;
	m.emission = t;
	m.emission_strength = emission_strength;
	return (m);
}
