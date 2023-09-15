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

color	sky_background(vec3 uv)
{
	return color_lerp(uv.y, WHITE, new_color(.5, .7, 1));
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