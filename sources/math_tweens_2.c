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

PFPN	cos_tween(PFPN a, PFPN b, PFPN t)
{
	PFPN	angle;

	angle = t * MYPI * 2;
	t = cos(angle);
	t = t / 2.0 + .5;
	return (lerpd(a, b, t));
}

PFPN	ease_in_out_cubic(PFPN a, PFPN b, PFPN t)
{
	if (t < 0.5)
		t = 4 * t * t * t;
	else
		t = 1 - pow(-2 * t + 2, 3) / 2;
	return (lerpd(a, b, t));
}

// Scale, and clamp x to 0..1 range
PFPN	smoothstep(PFPN edge0, PFPN edge1, PFPN x)
{
	x = clamp_((x - edge0) / (edge1 - edge0));
	return (x * x * (3.0 - 2.0 * x));
}
