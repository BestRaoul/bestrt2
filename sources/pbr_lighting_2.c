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

// proportion of facets alligned with light
PFPN	distribution_ggx(PFPN nDotH, PFPN roughness)
{
	PFPN	a;
	PFPN	a2;
	PFPN	denom;

	a = roughness * roughness;
	a2 = a * a;
	denom = nDotH * nDotH * (a2 - 1) + 1;
	denom = MYPI * denom * denom;
	return (fmax(a2 / denom, 0.0000001));
}

// self-shadowing
PFPN	geometry_smith(PFPN NdotV, PFPN NdotL, PFPN roughness)
{
	PFPN	r;
	PFPN	k;
	PFPN	ggx1;
	PFPN	ggx2;

	r = 1.0 + roughness;
	k = (r * r) / 8.0;
	ggx1 = NdotV / (NdotV * (1.0 - k) + k);
	ggx2 = NdotL / (NdotL * (1.0 - k) + k);
	return (ggx1 * ggx2);
}

//
vec3	fresnel_shlick(PFPN HdotV, PFPN R0)
{
	return (v_3(10 * R0 * (R0 + (1 - R0) * pow(1.0 - HdotV, 5.0))));
}

// 1 on failure OBSTRUCTED
Bool	check_obstruction(t_light *l, vec3 p)
{
	ray			check;
	interval	check_interval;

	if (v.solo_lighting)
		return (0);
	check_interval = (interval){0, INFINITY};
	if (l->is_dir)
	{
		check.orig = v_sub(p, v_scal(l->dir, 100));
		check.dir = v_norm(from_to(check.orig, p));
		check_interval.max = vec_dist(check.orig, p) - 0.001;
	}
	else
	{
		check.orig = l->transform.pos;
		check.dir = v_norm(from_to(l->transform.pos, p));
		check_interval.max = vec_dist(check.orig, p) - 0.001;
	}
	return (check_hit(&check, check_interval));
}
