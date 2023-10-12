/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

vec3    v_add(vec3 a, vec3 b)
{
    return v3(a.x+b.x, a.y+b.y, a.z+b.z);
}

vec3   from_to(vec3 a, vec3 b)
{
    return (v_sub(b, a));
}

vec3    v_sub(vec3 a, vec3 b)
{
    return v3(a.x-b.x, a.y-b.y, a.z-b.z);
}

vec3    v_mult(vec3 a, vec3 b)
{
    return v3(a.x*b.x, a.y*b.y, a.z*b.z);
}

vec3    v_div(vec3 a, vec3 b)
{
    return v3(a.x/b.x, a.y/b.y, a.z/b.z);
}


//defaults to 0
inline vec3    v3_base(vec3 in)
{
    return (vec3){in.x, in.y, in.z};
}

//all values to X
vec3    v_3(double x)
{
    return v3(x, x, x);
}

double	v_len(vec3 a)
{
	return (sqrtf(a.x * a.x + a.y * a.y + a.z * a.z));
}
