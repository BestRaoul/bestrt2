/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

vec3    get_ray_direction(int i, int j)
{
	double	aspect_ratio = (double)v.w/(double)v.h;
	double	correct_fov = tan(v.vfov * DEG2RAD * .5);

	vec3 p;
	p.x = ((2 * ((i +.5) / (double)v.w)) - 1) * aspect_ratio * correct_fov;
	p.y = (1 - (2 * ((j +.5) / (double)v.h))) * correct_fov;
	p.z = 1;
	p = v_norm(p);
	return (p);
}

vec3	ray_at(const ray *r, double t)
{
	return v_add(r->orig, v_scal(r->dir, t));
}

vec3	lerp(double t, vec3 a, vec3 b)
{
	return (vec3){
		(1-t)*a.x + t*b.x,
		(1-t)*a.y + t*b.y,
		(1-t)*a.z + t*b.z
	};
}

vec3    local_p(vec3 p, t_item item)
{
    vec3 relative_p = v_sub(p, item.pos);
    vec3 rotated_p = rotate3(relative_p, item.rot);
    vec3 scaled_p = v3(
        rotated_p.x / item.scale.x,
        rotated_p.y / item.scale.y,
        rotated_p.z / item.scale.z,
    );
    return scaled_p;
}
