/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raster.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	generate_tasks(t_split_task *tasks, int w_splits, int h_splits,
		int upscale)
{
	int	w_size;
	int	h_size;
	int	i;
	int	j;
	int	k;

	w_size = (v.w / v.upscale) / w_splits;
	h_size = (v.h / v.upscale) / h_splits;
	i = -1;
	while (++i < h_splits)
	{
		j = -1;
		while (++j < w_splits)
		{
			k = i * w_splits + j;
			tasks[k] = (t_split_task){
				(k % w_splits) * w_size,
				(k / w_splits) * h_size,
				w_size + ((v.w / v.upscale) % w_splits) * (j == w_splits -1),
				h_size + ((v.h / v.upscale) % h_splits) * (i == h_splits -1),
				1, //upscale,
				v.rendering_movie,
				0, 0};
		}
	}
}

bool	fullfill(t_split_task *task, int _split)
{
	while (task->_y < task->h_size)
	{
		while (task->_x < task->w_size)
		{
			render_pixel(task->_x + task->x_off, task->_y + task->y_off);
			task->_x += task->upscale;
			if (!task->noblock && get_elapsed(v.last_update) > 1000000.0
				/ TARGET_FPS)
			{
				print_progress(_split * task->w_size * task->h_size
					+ task->_x + task->_y * task->w_size);
				return (False);
			}
		}
		task->_x = 0;
		task->_y += task->upscale;
	}
	task->_y = 0;
	return (True);
}

void	show_progress(int _split, int max_splits,
		struct timeval frame_start)
{
	if (_split == max_splits)
	{
		print_progress(v.w * v.h);
		printf(" in %.2f ms\n", get_elapsed(frame_start) / 1000.0);
	}
}

void	print_progress(int pixel_count)
{
	PFPN		progress;
	const char	pm[21] = "####################";
	const char	no_pm[21] = "....................";
	char		s[100];

	progress = (PFPN)pixel_count / (v.w * v.h) * 100;
	write(1, "\33[2K\r", ft_strlen("\33[2K\r"));
	sprintf(s, "frame %d : [%s%.*s%s%.*s]  %.0f%%", v.steps_rendered, T_GRN,
		(int)progress / 5, pm, T_RESET,
		20 - (int)progress / 5, no_pm, progress);
	write(1, s, ft_strlen(s));
}
