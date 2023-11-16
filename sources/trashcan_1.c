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

void	maybe_apply_perturb(hit_record *rec)
{
	vec3	normal_rgb;
	vec3	perturbation;
	vec3	new_normal;

	normal_rgb = evaluate(&rec->mat.normal, rec->u, rec->v);
	if (v_eq(normal_rgb, v3(0.5, 0.5, 1.0)))
	{
		rec->old_normal = rec->normal;
		return ;
	}
	perturbation = v3(normal_rgb.x * 2 - 1, normal_rgb.y * 2 - 1, normal_rgb.z);
	perturbation = v_mult(perturbation, rec->mat.normal_strength);
	perturbation.z = fabs(perturbation.z);
	new_normal = perturb_normal(rec->normal, perturbation);
	rec->old_normal = rec->normal;
	rec->normal = new_normal;
}

vec3	perturb_normal(vec3 normal, vec3 perturbation)
{
	vec3	new_up;
	vec3	p_v;
	vec3	p_u;
	vec3	output;

	new_up = v3(0, 0, -1);
	if (normal.z > 0.99 || normal.z < -0.99)
		new_up = v3(1, 0, 0);
	p_v = v_cross(new_up, normal);
	p_v = v_norm(p_v);
	p_u = v_cross(normal, p_v);
	p_u = v_norm(p_u);
	output = normal;
	output = v_add(output, v_scal(p_u, perturbation.y));
	output = v_add(output, v_scal(p_v, -perturbation.x));
	output = v_add(output, v_scal(normal, perturbation.z));
	output = v_norm(output);
	return (output);
}

void	print_pos(vec3 pos, char *msg)
{
	printf("%s (%.4f, %.4f, %.4f)\n", msg, pos.x, pos.y, pos.z);
}

void	fswap(PFPN *__x, PFPN *__y)
{
	PFPN	__temp;

	__temp = *__x;
	*__x = *__y;
	*__y = __temp;
}

void	vswap(vec3 *__x, vec3 *__y)
{
	vec3	__temp;

	__temp = *__x;
	*__x = *__y;
	*__y = __temp;
}
