/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

color	sky_background(vec3 uv)
{
	return (color_lerp(uv.y, WHITE, new_color(.5, .7, 1)));
}

const color		g_sky_color_horizon = (vec3){.7, .8, 1.0};
const color		g_sky_color_zenith = (vec3){.3, .4, .7};
const color		g_ground_color = (vec3){.4, .4, .4};
const vec3		g_sun_direction = (vec3){-1, -1, -1};
const PFPN	g_sun_focus = 10;
const PFPN	g_sun_intensity = 1.0;

color	shit_sky_background(vec3 uv)
{
	vec3	dir;
	PFPN	sky_gradient_t;
	vec3	sky_gradient;
	PFPN	sun;
	PFPN	ground_to_sky_t;

	dir = v3(uv.x * 2.0 - 1, uv.y * 2.0 - 1, uv.z * 2.0 - 1);
	sky_gradient_t = pow(smoothstep(0, 0.4, dir.y), 0.35);
	sky_gradient = lerp(sky_gradient_t,
			g_sky_color_horizon, g_sky_color_zenith);
	sun = pow(maxd(0, v_dot(dir, g_sun_direction)), g_sun_focus)
		* g_sun_intensity;
	ground_to_sky_t = smoothstep(-0.01, 0, dir.y);
	return (v_add(lerp(ground_to_sky_t, g_ground_color, sky_gradient),
			v_3(sun * ground_to_sky_t >= 1)));
}

color	uv_background(vec3 uv)
{
	return (v_scal(v_add(uv, v_3(1)), 0.5));
}
