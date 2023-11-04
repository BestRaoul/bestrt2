/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tweens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

double	lerpd(double a, double b, double t)
{
	return (1-t) * a + t * b;
}

double	ping_pong(double a, double b, double t)
{
	double t2 = t < 0.5 ? t : 1 - t;
	t2 *= 2;
	return (1-t2) * a + t2 * b;
}

double	ping_pong_2(double a, double b, double t)
{
	double t2 = t < 0.5 ? t : 1 - t;
	t2 *= 2 * 2* t2;
	return (1-t2) * a + t2 * b;
}

double	sin_tween(double a, double b, double t)
{
	double angle = t * MYPI * 2;
	t = sin(angle);
	t = t / 2.0 + .5;
	return (lerpd(a, b, t));
}

double	cos_tween(double a, double b, double t)
{
	double angle = t * MYPI * 2;
	t = cos(angle);
	t = t / 2.0 + .5;
	return (lerpd(a, b, t));
}

double	easeInOutCubic(double a, double b, double t)
{
	t = t < 0.5 ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2;
	return lerpd(a, b, t);
}

void	add_motion(double *value, double start_value, double end_value, double (*tween)(double, double , double))
{
	motion	*new_motions = gc_malloc((v.motion_count + 1) * sizeof(motion));
	for (int i=0; i<v.motion_count; i++)
		new_motions[i] = v.motions[i];
	new_motions[v.motion_count] = (motion){value, start_value, end_value, tween};
	v.motions = new_motions;
	v.motion_count++;
}
