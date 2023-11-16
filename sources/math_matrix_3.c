/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_matrix.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

// Augmented matrix [in | I], where I is the 4x4 identity matrix
static void	init(PFPN aug[4][8], const m4x4 in)
{
	int	i;
	int	j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			aug[i][j] = in[i][j];
			aug[i][j + 4] = (i == j);
			j++;
		}
		i++;
	}
}

// Perform row operations to get the upper triangular form
static void	upper_triangular_form(PFPN aug[4][8])
{
	PFPN	ratio;
	int		i;
	int		j;
	int		k;

	i = 0;
	while (i < 4)
	{
		j = i + 1;
		while (j < 4)
		{
			ratio = aug[j][i] / aug[i][i];
			k = 0;
			while (k < 8)
			{
				aug[j][k] -= ratio * aug[i][k];
				k++;
			}
			j++;
		}
		i++;
	}
}

static void	back_substition(PFPN aug[4][8])
{
	PFPN	ratio;
	int		i;
	int		j;
	int		k;

	i = 3;
	while (i > 0)
	{
		j = i - 1;
		while (j >= 0)
		{
			ratio = aug[j][i] / aug[i][i];
			k = 0;
			while (k < 8)
			{
				aug[j][k] -= ratio * aug[i][k];
				k++;
			}
			j--;
		}
		i--;
	}
}

static void	normalize(PFPN aug[4][8])
{
	PFPN	divisor;
	int		i;
	int		j;
	int		k;

	i = 0;
	while (i < 4)
	{
		divisor = aug[i][i];
		j = 0;
		while (j < 8)
		{
			aug[i][j] /= divisor;
			j++;
		}
		i++;
	}
}

void	inverse(const m4x4 in, m4x4 out)
{
	PFPN	aug[4][8];
	int		i;
	int		j;

	init(aug, in);
	upper_triangular_form(aug);
	back_substition(aug);
	normalize(aug);
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			out[i][j] = aug[i][j + 4];
			j++;
		}
		i++;
	}
}
