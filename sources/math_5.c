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

//unused
void	mm_(const m4x4 a, const m4x4 b, m4x4 c)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            c[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

s_m4	mm(const m4x4 a, const m4x4 b)
{
    s_m4    c;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            c.mat[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                c.mat[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

vec3	mult_point_matrix(const vec3 in, const m4x4 M)
{ 
	vec3 out;
    //out = in * M;
    out.x   = in.x * M[0][0] + in.y * M[1][0] + in.z * M[2][0] + /* in.z = 1 */ M[3][0]; 
    out.y   = in.x * M[0][1] + in.y * M[1][1] + in.z * M[2][1] + /* in.z = 1 */ M[3][1]; 
    out.z   = in.x * M[0][2] + in.y * M[1][2] + in.z * M[2][2] + /* in.z = 1 */ M[3][2]; 
    double w = in.x * M[0][3] + in.y * M[1][3] + in.z * M[2][3] + /* in.z = 1 */ M[3][3]; 
 
    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    if (w != 1) { 
        out.x /= w; 
        out.y /= w; 
        out.z /= w; 
    } 
	return out;
}
