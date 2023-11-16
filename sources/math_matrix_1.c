/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_matrix.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

//multiply mattrix with matrix
s_m4	mm(const m4x4 a, const m4x4 b)
{
	s_m4	c;
	int		i;
	int		j;
	int		k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			c.mat[i][j] = 0;
			k = 0;
			while (k < 4)
			{
				c.mat[i][j] += a[i][k] * b[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
	return (c);
}

//multiply matrix with point
vec3	mult_point_matrix(const vec3 in, const m4x4 M)
{
	vec3	out;
	PFPN	w;

	w = in.x * M[0][3] + in.y * M[1][3] + in.z * M[2][3] + M[3][3];
	out.x = in.x * M[0][0] + in.y * M[1][0] + in.z * M[2][0] + M[3][0];
	out.y = in.x * M[0][1] + in.y * M[1][1] + in.z * M[2][1] + M[3][1];
	out.z = in.x * M[0][2] + in.y * M[1][2] + in.z * M[2][2] + M[3][2];
	if (w != 1)
	{
		out.x /= w;
		out.y /= w;
		out.z /= w;
	}
	return (out);
}

// // normalize if w is different than 1
// //(convert from homogeneous to Cartesian coordinates)
// //unused
void	multiply_matrix_vector(const m4x4 mat, const vec3 in, vec3 *out)
{
	PFPN	w;

	out->x = mat[0][0] * in.x + mat[0][1] * in.y + mat[0][2] * in.z
		+ /* 1  this is literally the best feature there is */ mat[0][3];
	out->y = mat[1][0] * in.x + mat[1][1] * in.y + mat[1][2] * in.z
		+ /* 1 */ mat[1][3];
	out->z = mat[2][0] * in.x + mat[2][1] * in.y + mat[2][2] * in.z
		+ /* 1 */ mat[2][3];
	w = mat[3][0] * in.x + mat[3][1] * in.y + mat[3][2] * in.z
		+ /* 1 */ mat[3][3];
	if (w != 1)
	{
		out->x /= w;
		out->y /= w;
		out->z /= w;
	}
}

void	print_mx4(m4x4 m)
{
	int	i;

	printf("MATRIX 4X4 : {\n");
	i = 0;
	while (i < 4)
	{
		printf("% 4.4f, % 4.4f, % 4.4f, % 4.4f;", m[i][0], m[i][1], m[i][2],
			m[i][3]);
		if (i != 3)
			printf("\n");
		i++;
	}
	printf("}\n");
}
