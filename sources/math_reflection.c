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

vec3	reflect(vec3 v, vec3 n)
{
	return (v_sub(v, v_scal(n, 2 * v_dot(v, n))));
}

// reflects
// if under surface pops it up above
vec3	reflect_safe(vec3 v, vec3 n, vec3 old_n)
{
	vec3	reflected;
	vec3	new_v;

	reflected = reflect(v, n);
	return (reflected);
	if (v_dot(reflected, old_n) < 0)
	{
		new_v = v_scal(reflected, -1);
		reflected = reflect(new_v, old_n);
	}
	return (reflected);
}

vec3	refract(vec3 uv, vec3 n, PFPN etai_over_etat)
{
	PFPN	cos_theta;
	vec3	r_out_perp;
	vec3	r_out_parallel;

	cos_theta = fmin(v_dot(v_scal(uv, -1), n), 1.0);
	r_out_perp = v_scal(v_add(uv, v_scal(n, cos_theta)), etai_over_etat);
	r_out_parallel = v_scal(n, -sqrt(fabs(1.0 - length_squared(r_out_perp))));
	return (v_add(r_out_perp, r_out_parallel));
}

// Use Schlick's approximation for reflectance.
PFPN	reflectance(PFPN cosine, PFPN ref_idx)
{
	PFPN	r0;

	r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * pow((1 - cosine), 5));
}

PFPN	realistic_specular(PFPN ior)
{
	PFPN	r0;

	r0 = (1 - ior) / (1 + ior);
	r0 = r0 * r0;
	return (r0 / 0.08);
}
