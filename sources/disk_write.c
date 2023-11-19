/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disk.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include <dirent.h>

#define NTFY_SERVER "0.0.0.0:80/balls"

const uint8_t	g_bmp_header[54] = {
	'B', 'M',		// BMP file signature
	0, 0, 0, 0,		// File size (placeholder)
	0, 0, 0, 0,		// Reserved
	54, 0, 0, 0,	// Pixel data offset
	40, 0, 0, 0,	// Header size
	0, 0, 0, 0,		// Image width (placeholder)
	0, 0, 0, 0,		// Image height (placeholder)
	1, 0,			// Number of color planes
	24, 0,			// Bits per pixel (24-bit RGB)
	0, 0, 0, 0,		// Compression method (none)
	0, 0, 0, 0,		// Image size (placeholder)
	0, 0, 0, 0,		// Horizontal resolution (pixels per meter)
	0, 0, 0, 0,		// Vertical resolution (pixels per meter)
	0, 0, 0, 0,		// Number of colors in palette
	0, 0, 0, 0,		// Important colors
};

static inline void	actually_write_bmp(FILE *file, int width, int height,
	unit8_color *pixels)
{
	uint8_t		b;
	uint8_t		g;
	uint8_t		r;
	int			i;

	i = 0;
	while (i < width * height)
	{
		b = pixels[i].b;
		g = pixels[i].g;
		r = pixels[i].r;
		fwrite(&b, sizeof(uint8_t), 1, file);
		fwrite(&g, sizeof(uint8_t), 1, file);
		fwrite(&r, sizeof(uint8_t), 1, file);
		i++;
	}
}

static inline void	set_header(uint8_t bmp_header[54], int width, int height)
{
	uint32_t	file_size;
	uint32_t	image_size;

	memcpy(bmp_header, g_bmp_header, sizeof(uint8_t) * 54);
	file_size = 54 + (width * height * 3);
	image_size = width * height * 3;
	bmp_header[2] = (uint8_t)(file_size);
	bmp_header[3] = (uint8_t)(file_size >> 8);
	bmp_header[4] = (uint8_t)(file_size >> 16);
	bmp_header[5] = (uint8_t)(file_size >> 24);
	bmp_header[18] = (uint8_t)(width);
	bmp_header[19] = (uint8_t)(width >> 8);
	bmp_header[20] = (uint8_t)(width >> 16);
	bmp_header[21] = (uint8_t)(width >> 24);
	bmp_header[22] = (uint8_t)(height);
	bmp_header[23] = (uint8_t)(height >> 8);
	bmp_header[24] = (uint8_t)(height >> 16);
	bmp_header[25] = (uint8_t)(height >> 24);
	bmp_header[34] = (uint8_t)(image_size);
	bmp_header[35] = (uint8_t)(image_size >> 8);
	bmp_header[36] = (uint8_t)(image_size >> 16);
	bmp_header[37] = (uint8_t)(image_size >> 24);
}

// Function to write a BMP image
void	write_bmp(const char *filename, int width, int height,
	unit8_color *pixels)
{
	FILE		*file;
	uint8_t		bmp_header[54];

	file = fopen(filename, "wb");
	if (!file)
		return ((void)perror("Error opening file"));
	set_header(bmp_header, width, height);
	fwrite(bmp_header, sizeof(uint8_t), 54, file);
	actually_write_bmp(file, width, height, pixels);
	fclose(file);
}

static inline void	set_pixels(unit8_color *pixels)
{	
	color		c;
	int			y;
	int			x;

	y = 0;
	while (y < v.h)
	{
		x = 0;
		while (x < v.w)
		{
			c = rgb2color(get_pixel(x, v.h - y));
			pixels[y * v.w + x]
				= (unit8_color){c.x * 255, c.y * 255, c.z * 255};
			x++;
		}
		y++;
	}
}

void	write_img(void)
{
	static int	count = 0;
	DIR			*dir;
	char		filename[100];
	unit8_color	*pixels;

	dir = opendir(OUTFOLDER);
	if (!dir)
		return ((void)printf("no such directory \"%s\"\n", OUTFOLDER));
	closedir(dir);
	write(1, "writing img..", 14);
	sprintf(filename, "%s/img_%s.bmp", OUTFOLDER, get_current_date_time());
	// sprintf(filename, filename, count);
	pixels = gc_malloc(v.w * v.h * sizeof(unit8_color));
	set_pixels(pixels);
	write_bmp(filename, v.w, v.h, pixels);
	write(1, "done!", 5);
	gc_free(pixels);
	count++;
}
