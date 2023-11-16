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

// 0.0 to 1.0 using PCG hash
PFPN	random_pfpn(void)
{
	static unsigned int	state;
	unsigned int		result;

	state = state * 747796405 + 2891336453;
	result = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
	result = (result >> 22) ^ result;
	return (result / 4294967295.0);
}

PFPN	random_normal_distribution(void)
{
	PFPN	theta;
	PFPN	rho;

	theta = 2 * 3.1415926 * random_pfpn();
	rho = sqrt(-2 * log(random_pfpn()));
	return (rho * cos(theta));
}

PFPN	random_pfpn_l(PFPN min, PFPN max)
{
	return (min + (max - min) * random_pfpn());
}

vec3	pixel_sample_square(void)
{
	PFPN	px;
	PFPN	py;

	px = -0.5 + random_pfpn();
	py = -0.5 + random_pfpn();
	return (v_add(v_scal(v.pixel_delta_u, px), v_scal(v.pixel_delta_v, py)));
}

vec3	random_in_unit_disk(void)
{
	vec3	p;

	while (True)
	{
		p = v3(random_pfpn() * 2 - 1, random_pfpn() * 2 - 1, 0);
		if (length_squared(p) < 1)
			return (p);
	}
	return (v3(0, 0, 0));
}
