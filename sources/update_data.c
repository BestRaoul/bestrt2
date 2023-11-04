/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upate_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	update_delta_time(void)
{
	struct timeval current;
	gettimeofday(&current, 0);
	int elapsed_ms = ((current.tv_sec - v.last_update.tv_sec) * 1000000	+ current.tv_usec - v.last_update.tv_usec);
	v.delta_time = elapsed_ms / 1000000.0;
	v.last_update = current;
}

void	apply_motions(void)
{
	static int	was_enabled = 0;
	double		t;
	double		t_offset;
	motion		m;
	int			i;

	if (!was_enabled && v.motion_enabled)
		t_offset = v.time_passed;
	if (v.motion_enabled)
	{
		t = (v.time_passed - t_offset) - (int)(v.time_passed - t_offset) / 1;
		i = 0;
		while (i < v.motion_count)
		{
			m = v.motions[i];
			*m.value = m.tween(m.start_val, m.end_val, t);
			i++;
		}
	}
	was_enabled = v.motion_enabled;
}

void	update_lamp_stats(void)
{
	t_light	*lamp;
	int		i;

	i = 0;
	while (i < v.light_count)
	{
		lamp = &v.lights[i];
		lamp->intensity = v_len(lamp->transform.scale);
		lamp->dir = rotate3(v3(1, 0, 0), lamp->transform.rot);
		i++;
	}
}

void	update_tfm_matrices(void)
{
	t_item	*item;
	int		i;

	i = 0;
	while (i < v.item_count)
	{
		item = &v.items[i];
		set_transform_matrix(&item->transform, item->fwd, item->bck);
		i++;
	}
}

