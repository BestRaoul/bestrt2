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

void	draw_raw(int x, int y, color c)
{
	char	*pixel;

	pixel = v.img.addr + v.img.line_len * y + x * (v.img.bpp / 8);
	*(unsigned int *)pixel = color2rgb(c);
}

int	get_pixel(int x, int y)
{
	char	*pixel;

	pixel = v.img.addr + v.img.line_len * y + x * (v.img.bpp / 8);
	return *(unsigned int *)pixel;
}

//0 if not inbouds, 1 on success
int	draw_inbounds(int x, int y, color c)
{
	if (!in_bounds(x, y)) return 0;

	draw_raw(x, y, c);
	return 1;
}

int draw_v_inbounds(vec3 p, color c)
{
	return draw_inbounds(p.x, p.y, c);
}

//0 if not inbouds, 1 on success
int	draw_v_inheat(vec3 p, color c)
{
	if (!v_in_bounds(p)) return 0;

	if (!check_heat(p))
	{
		set_heat(p);
		draw_raw(p.x, p.y, c);
	}
	return 1;
}

//might be some error because this seems to clear a bit too much..
//if you test with (v.w-1) the last line is only 2/3 not cleared
void	clear_img(t__img img)
{
	memset(img.addr, 0, v.w * v.h * sizeof(unsigned int));
}

void gradientContrastAdjustment(float* image, int width, int height, float contrastFactor) {
    int x, y;

    // Calculate gradients using central differences
    for (y = 1; y < height - 1; y++) {
        for (x = 1; x < width - 1; x++) {
            float dx = image[y * width + x + 1] - image[y * width + x - 1];
            float dy = image[(y + 1) * width + x] - image[(y - 1) * width + x];
            float gradient = sqrt(dx * dx + dy * dy);
            
            // Adjust pixel intensity based on gradient
            image[y * width + x] *= pow(gradient, contrastFactor);
        }
    }
}

color	hdr_tone(color c)
{
   c = v_div(c, v_add(c, v_3(1.0)));
   return c;
}

color	aces_tone(color c)
{
   	// Calculate luminance
    double Y = 0.299 * c.x + 0.587 * c.y + 0.114 * c.z;
    
    // Adjust luminance
    double Y_mapped = ACES(Y);
    
    // Apply color balance
    double R_mapped = c.x * (Y_mapped / Y);
    double G_mapped = c.y * (Y_mapped / Y);
    double B_mapped = c.z * (Y_mapped / Y);

   return v3(R_mapped, G_mapped, B_mapped);
}

color	gamma_correct(color c)
{
    // Clamp values to [0, 1] and apply gamma correction if needed
    //c.x = clamp((interval){0.0, 1.0}, c.x);
    //c.y = clamp((interval){0.0, 1.0}, c.y);
    //c.z = clamp((interval){0.0, 1.0}, c.z);
    
	c.x = pow(c.x, 1.0/2.2);
	c.y = pow(c.y, 1.0/2.2);
	c.z = pow(c.z, 1.0/2.2);

	return c;
}

void	draw_gamma_corrected(int x, int y, color c)
{
	draw_raw(x, y, c);
	return;
	
	c = aces_tone(c);
	c = gamma_correct(c);

	draw_raw(x, y, c);
}
