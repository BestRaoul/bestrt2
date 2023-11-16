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

Bool	contains(interval _t, PFPN x)
{
	return (_t.min <= x && x <= _t.max);
}

Bool	surrounds(interval _t, PFPN x)
{
	return (_t.min < x && x < _t.max);
}

PFPN	clamp(interval _t, PFPN x)
{
	if (x < _t.min)
		return (_t.min);
	if (x > _t.max)
		return (_t.max);
	return (x);
}

/* 0 to 1 */
PFPN	clamp_(PFPN x)
{
	if (x < 0.0)
		return (0.0);
	if (x > 1.0)
		return (1.0);
	return (x);
}
