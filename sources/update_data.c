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
