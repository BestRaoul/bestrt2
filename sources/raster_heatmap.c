/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raster.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

// might have error same as clear_img
// memset(img.addr, 0, v.w * v.h * sizeof(unsigned int));
void	reset_heatmap(void)
{
	memset(v.dist_heatmap[0], -1., v.w * v.h * sizeof(PFPN));
}

Bool	check_heat(vec3 pos)
{
	int	x;
	int	y;

	x = pos.x;
	y = pos.y;
	return (pos.z < 0 || (pos.z > v.dist_heatmap[x][y]));
}

void	set_heat(vec3 pos)
{
	int	x;
	int	y;

	x = pos.x;
	y = pos.y;
	v.dist_heatmap[x][y] = pos.z;
}
