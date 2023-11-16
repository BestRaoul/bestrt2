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

void	default_crusor(void)
{
	set_cursor(XC_crosshair);
}

void	mode_cursor(int mode)
{
	if (mode == MOVE)
		set_cursor(XC_fleur);
	if (mode == ROTATE)
		set_cursor(XC_exchange);
	if (mode == SCALE)
		set_cursor(XC_sizing);
}

PFPN	g_kernel[5][5] = {
{1.0, 4.0, 6.0, 4.0, 1.0},
{4.0, 16.0, 24.0, 16.0, 4.0},
{6.0, 24.0, 36.0, 24.0, 6.0},
{4.0, 16.0, 24.0, 16.0, 4.0},
{1.0, 4.0, 6.0, 4.0, 1.0}};

#define KCENTER 2

static inline void	something(vec3 at, vec3 ij, const texture *self, color *sum)
{
	color	pixe_value;
	int		x_offset;
	int		y_offset;
	int		i;
	int		j;

	i = ij.x;
	j = ij.y;
	x_offset = at.x + i * 3 - KCENTER;
	y_offset = at.y + j * 3 - KCENTER;
	if (x_offset >= 0 && x_offset < self->image_width && y_offset >= 0
		&& y_offset < self->image_height)
	{
		pixe_value = self->value(x_offset / self->image_width, y_offset
				/ self->image_height, self);
		sum->x += g_kernel[i][j] * pixe_value.x;
		sum->y += g_kernel[i][j] * pixe_value.y;
		sum->z += g_kernel[i][j] * pixe_value.z;
	}
}

color	gaussian_blur(int x, int y, const texture *self)
{
	color	sum;
	int		i;
	int		j;

	sum = v3(0.0);
	i = 0;
	while (i < 5)
	{
		j = 0;
		while (j < 5)
		{
			something(v3(x, y), v3(i, j), self, &sum);
			j++;
		}
		i++;
	}
	return (v_scal(sum, 1.0 / 256.0f));
}

// int		ft_find(char c, char *str)
// {
// 	int	i;
//
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (c == str[i])
// 			return (i);
// 	}
// 	return (-1);
// }
//
// char	**inplace_split(char *str, char *sep)
// {
// 	char	**splits;
// 	int		c;
// 	int		i;
// 	int		previous_is;
//
// 	c = 0;
// 	previous_is = 1;
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (ft_find(str[i], sep) == -1 && previous_is)
// 			c++;
// 		previous_is = ft_find(str[i], sep) == -1;
// 		i++;
// 	}
// 	splits = gc_malloc(sizeof(char *) * (c + 1));
// 	previous_is = 1;
// 	c = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (ft_find(str[i], sep) == -1 && previous_is)
// 			splits[c++] = ft_strdup()
// 		previous_is = ft_find(str[i], sep) == -1;
// 		i++;
// 	}
// }

char	**_gc_add_2d(char	**ptr_2d)
{
	int	i;

	garbage_collector(ADD, ptr_2d);
	i = 0;
	while (ptr_2d[i] != NULL)
		garbage_collector(ADD, ptr_2d[i++]);
	return (ptr_2d);
}
