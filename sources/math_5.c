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

// Define the constants for the ACES tone mapping function
#define A 2.51
#define B 0.03
#define C 2.43
#define D 0.59
#define E 0.14

// ACES tone mapping function
PFPN	aces(PFPN x)
{
	return ((x * (A * x + B)) / (x * (C * x + D) + E));
}

PFPN	linear_to_gamma(PFPN linear_component)
{
	return (sqrt(linear_component));
}

bool	near_zero(vec3 e)
{
	PFPN	s;

	s = 1e-8;
	return ((fabs(e.x) < s) && (fabs(e.y) < s) && (fabs(e.z) < s));
}

bool	close_enough(PFPN __x)
{
	return (fabs(__x) < 1e-8);
}
