/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

// might be some error because this seems to clear a bit too much..
// if you test with (v.w-1) the last line is only 2/3 not cleared
void	clear_img(t__img img)
{
	memset(img.addr, 0, v.w * v.h * sizeof(unsigned int));
}

color	hdr_tone(color c)
{
	c = v_div(c, v_add(c, v_3(1.0)));
	return (c);
}

color	aces_tone(color c)
{
	double	y;
	double	y_mapped;
	double	r_mapped;
	double	g_mapped;
	double	b_mapped;

	y = 0.299 * c.x + 0.587 * c.y + 0.114 * c.z;
	y_mapped = ACES(y);
	r_mapped = c.x * (y_mapped / y);
	g_mapped = c.y * (y_mapped / y);
	b_mapped = c.z * (y_mapped / y);
	return (v3(r_mapped, g_mapped, b_mapped));
}

// Clamp values to [0, 1] and apply gamma correction if needed
color	gamma_correct(color c)
{
	c.x = clamp((interval){0.0, 1.0}, c.x);
	c.y = clamp((interval){0.0, 1.0}, c.y);
	c.z = clamp((interval){0.0, 1.0}, c.z);
	c.x = pow(c.x, 1.0 / 2.2);
	c.y = pow(c.y, 1.0 / 2.2);
	c.z = pow(c.z, 1.0 / 2.2);
	return (c);
}

void	draw_gamma_corrected(int x, int y, color c)
{
	draw_raw(x, y, c);
	return ;
	c = aces_tone(c);
	c = gamma_correct(c);
	draw_raw(x, y, c);
}

// void	gradientContrastAdjustment(float *image, int width, int height,
// 		float contrastFactor)
// {
// 	float	dx;
// 	float	dy;
// 	float	gradient;
//
// 	int x, y;
// 	// Calculate gradients using central differences
// 	for (y = 1; y < height - 1; y++)
// 	{
// 		for (x = 1; x < width - 1; x++)
// 		{
// 			dx = image[y * width + x + 1] - image[y * width + x - 1];
// 			dy = image[(y + 1) * width + x] - image[(y - 1) * width + x];
// 			gradient = sqrt(dx * dx + dy * dy);
// 			// Adjust pixel intensity based on gradient
// 			image[y * width + x] *= pow(gradient, contrastFactor);
// 		}
// 	}
// }

