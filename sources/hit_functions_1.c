/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

bool	hit(const ray *r, const interval ray_t, hit_record *rec)
{
	bool		hit_anything;
	PFPN		closest_so_far;
	hit_record	temp_rec;
	int			alpha;
	int			i;

	hit_anything = False;
	closest_so_far = ray_t.max;
	i = -1;
	while (++i < v.item_count)
	{
		if (v.items[i].hit(r, (interval){ray_t.min, closest_so_far},
			&temp_rec, &v.items[i]))
		{
			alpha = .5 + evaluate_bw(&(temp_rec.mat.alpha), temp_rec.u,
					temp_rec.v);
			if (alpha == 0)
				continue ;
			hit_anything = True;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
	}
	return (hit_anything);
}

bool	info_hit(const ray *r, const interval ray_t, hit_record *rec)
{
	bool		hit_anything;
	PFPN		closest_so_far;
	hit_record	temp_rec;
	int			alpha;
	int			i;

	hit_anything = False;
	closest_so_far = ray_t.max;
	i = -1;
	while (++i < v.item_count)
	{
		if (v.items[i].hit(r, (interval){ray_t.min, closest_so_far},
			&temp_rec, &v.items[i]))
		{
			alpha = .5 + evaluate_bw(&(temp_rec.mat.alpha), temp_rec.u,
					temp_rec.v);
			if (alpha == 0)
				continue ;
			hit_anything = True;
			closest_so_far = temp_rec.t;
			temp_rec.item = &(v.items[i]);
			*rec = temp_rec;
		}
	}
	return (hit_anything);
}

bool	check_hit(const ray *r, const interval ray_t)
{
	hit_record	temp_rec;
	int			alpha;
	int			i;

	i = 0;
	while (i < v.item_count)
	{
		if (v.items[i].hit(r, ray_t, &temp_rec, &v.items[i]))
		{
			alpha = .5 + evaluate_bw(&(temp_rec.mat.alpha), temp_rec.u,
					temp_rec.v);
			if (alpha == 0)
			{
				i++;
				continue ;
			}
			return (True);
		}
		i++;
	}
	return (False);
}
