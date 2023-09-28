/*                                                        :::      ::::::::   */
/*   raytrace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

double maxd(double a, double b)
{
	if (a > b) return a;
	return b;
}

color	sky_background(vec3 uv)
{
	return color_lerp(uv.y, WHITE, new_color(.5, .7, 1));
}

color	shit_sky_background(vec3 uv)
{
	vec3 dir = v3(uv.x*2.0 - 1, uv.y*2.0 - 1, uv.z*2.0 - 1);
	double skyGradientT = pow(smoothstep(0, 0.4, dir.y), 0.35);
	vec3 skyGradient = lerp(skyGradientT, v.skyColorHorizon, v.skyColorZenith);
	double sun = pow(maxd(0, v_dot(dir, v.sunDirection)), v.sunFocus) * v.sunIntensity;

	double groundToSkyT = smoothstep(-0.01, 0, dir.y);
	double sunMask = groundToSkyT >= 1;
	return v_add(lerp(groundToSkyT, v.groundColor, skyGradient),
			v_3(sun * sunMask));
}

color	black_background(vec3 uv)
{
	return BLACK;
}

color	white_background(vec3 uv)
{
	return WHITE;
}

color	uv_background(vec3 uv)
{
	return new_color(uv.x, uv.y, uv.z);
}