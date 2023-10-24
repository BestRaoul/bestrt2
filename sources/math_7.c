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

#include <limits.h>

//RANDOM

//0.0 to 1.0 using PCG hash
double random_double(void) 
{
    static unsigned int state;
    state = state * 747796405 + 2891336453;
    unsigned int result = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
    result = (result >> 22) ^ result;
	return result / 4294967295.0;
}

double random_normal_distribution(void)
{
    double theta = 2 * 3.1415926 * random_double();
    double rho = sqrt(-2 * log(random_double()));
    return rho * cos(theta);
}

double random_double_l(double min, double max)
{
	return min + (max-min)*random_double();
}

vec3	pixel_sample_square(void)
{
	double px = -0.5 + random_double();
	double py = -0.5 + random_double();
	return v_add(v_scal(v.pixel_delta_u, px), v_scal(v.pixel_delta_v, py));
}

vec3 random_in_unit_disk(void) {
    for (int i=0; i<1000; i++)
    {
        vec3 p = v3(random_double()*2 -1, random_double()*2 -1, 0);
        if (length_squared(p) < 1)
            return p;
    }
    return v3(0,0,0);
}

vec3 defocus_disk_sample() {
    // Returns a random point in the camera defocus disk.
    vec3 p = random_in_unit_disk();
    return v_add(v_add(v.camera_pos, v_scal(v.defocus_disk_u, p.x)), v_scal(v.defocus_disk_v, p.y));
}

vec3 random_v3()
{
    return v3(random_double(), random_double(), random_double());
}

vec3 random_v3_l(double min, double max)
{
    return v3(random_double_l(min,max), random_double_l(min,max), random_double_l(min,max));
}

vec3 random_in_unit_sphere()
{
    while (1)
    {
        vec3 p = random_v3_l(-1,1);
        if (length_squared(p) < 1)
            return p;
    }
}

vec3 random_unit_vector()
{
    //fix why even try to land it inside the unit sphere when you can instantly normalize it
    return v_norm(random_in_unit_sphere());
}

vec3    random_on_hemisphere(vec3 normal)
{
    vec3 on_unit_sphere = random_unit_vector();
    if (v_dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return v_scal(on_unit_sphere, -1);
}
