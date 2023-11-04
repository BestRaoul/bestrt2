/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scribes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	scribe(char *str, int x, int y, color c)
{
	mlx_string_put(v.mlx, v.win, x, y, color2rgb(c), str);
}

// Careful! there is a max of 64 char to be scribed
void	scribe_pos(char *tag, vec3 pos, int x, int y, color c)
{
	char	str[64];

	memset(str, '\0', 64);
	sprintf(str, "%s (%d, %d)", tag, (int)pos.x, (int)pos.y);
	scribe(str, x, y, c);
}

void	scribe_v3d(char *tag, vec3 pos, int x, int y, color c)
{
	char	str[64];

	memset(str, '\0', 64);
	sprintf(str, "%s (%.2f, %.2f, %.2f)", tag, pos.x, pos.y, pos.z);
	scribe(str, x, y, c);
}

void	scribe_num(char *format, int n, int x, int y, color c)
{
	char	str[64];

	memset(str, '\0', 64);
	sprintf(str, format, n);
	scribe(str, x, y, c);
}

void	scribe_dub(char *format, double d, int x, int y, color c)
{
	char	str[64];

	memset(str, '\0', 64);
	sprintf(str, format, d);
	scribe(str, x, y, c);
}
