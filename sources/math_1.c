/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

double	smoothstep (double edge0, double edge1, double x) {
   // Scale, and clamp x to 0..1 range
   x = clamp_((x - edge0) / (edge1 - edge0));

   return x * x * (3.0 - 2.0 * x);
}

double	distance(vec3 a, vec3 b)
{
	return sqrt((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y)+(b.z-a.z)*(b.z-a.z));
}

int	sign(int n)
{
	if (n >= 0)
		return (1);
	return (-1);
}

double	signd(double n)
{
	if (n < 0)
		return -1;
	return 1;
}

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

double	mind(double a, double b)
{
	if (a > b)
		return (a);
	return (b);
}

double	maxd(double a, double b)
{
	if (a > b)
		return (a);
	return (b);
}

int	in_bounds(int x, int y)
{
	if (x < 0 || y < 0 || x >= v.w || y >= v.h)
		return (0);
	return (1);
}

int	v_in_bounds(vec3 pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.x >= v.w || pos.y >= v.h)
		return (0);
	return (1);
}

double	fclamp(double x, double min, double max)
{
	if (x < min)
		return (min);
	else if (x > max)
		return (max);
	return (x);
}
