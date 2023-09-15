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

vec3	line_intersection(vec3 A, vec3 B, vec3 C, vec3 D)
{
    // Line AB represented as a1x + b1y = c1
    double a1 = B.y - A.y;
    double b1 = A.x - B.x;
    double c1 = a1*(A.x) + b1*(A.y);
 
    // Line CD represented as a2x + b2y = c2
    double a2 = D.y - C.y;
    double b2 = C.x - D.x;
    double c2 = a2*(C.x)+ b2*(C.y);
 
    double determinant = a1*b2 - a2*b1;
 
    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        return v3(-1,-1);
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        return v3(x, y);
    }
}

//sometimes gives wrong Z distance, PROBABLY WHEN one point is behind, or smth similar
vec3	project_onto_screen_limits(vec3 p, vec3 dir)
{
	vec3	intersections[2]; //-1, -1 means invalid (parallel)

	intersections[0] = v3(-1, -1);
	intersections[1] = v3(-1, -1);

	//X
	if (dir.x > 0)
		intersections[0] = line_intersection(p, v_add(p, dir), v3(v.w-1, 0), v3(v.w-1, 1));
	else if (dir.x < 0)
		intersections[0] = line_intersection(p, v_add(p, dir), v3(0, 0), v3(0, 1));
	//Y
	if (dir.y < 0)
		intersections[1] = line_intersection(p, v_add(p, dir), v3(0), v3(1));
	else if (dir.y > 0)
		intersections[1] = line_intersection(p, v_add(p, dir), v3(0, v.h-1), v3(1, v.h-1));


    vec3    r = intersections[v_in_bounds(intersections[1])];
    r.z = dir.x != 0 ? (r.x-p.x)/dir.x : (r.y-p.y)/dir.y;
    //quick fix for infinitelines
    if (r.z < 0)
    {
		//printf("weird Z: %f\n", r.z);
		r.z = 0;
	}
    return r;
}
