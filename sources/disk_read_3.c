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

static inline void	acutally_read_palette(FILE *file,
	t_bmp_header *header, int palette[256])
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	__reserved;
	int		i;

	i = -1;
	while (++i < 256)
	{
		fread(&r, 1, 1, file);
		fread(&g, 1, 1, file);
		fread(&b, 1, 1, file);
		fread(&__reserved, 1, 1, file);
		palette[i] = new_rgb(r, g, b);
	}
}

Bool	read_palette(FILE *file, t_bmp_header *header, int palette[256])
{
	if (header->bpp == 8)
		acutally_read_palette(file, header, palette);
	if (header->bpp == 1 || header->bpp == 4)
	{
		NOT_IMPLEMENTED("bmp 1-bit and 4-bit support");
		fclose(file);
		return (False);
	}
	else if (header->bpp == 8 || header->bpp == 24)
	{
		if (header->bpp == 8)
		{
			NOT_IMPLEMENTED("add 8-bit bmp read from palette");
			return (False);
		}
	}
	else
	{
		fprintf(stderr, "Error: Unsupported BMP format \
		(bits per pixel %u != 24)\n", header->bpp);
		fclose(file);
		return (False);
	}
	return (True);
}

void	init_pixel_read(t_bmp_header *header, bmp_read *r)
{
	int	i;

	r->widht = header->width;
	r->height = header->height;
	r->pixels = (int **)gc_malloc(sizeof(int *) * r->widht);
	i = 0;
	while (i < r->height)
	{
		(r->pixels)[i] = (int *)gc_malloc(sizeof(int) * r->widht);
		i++;
	}
}

static int	read_rgb(FILE *file, t_bmp_header *header, int palette[256])
{
	uint8_t	blue;
	uint8_t	green;
	uint8_t	red;

	if (header->bpp == 24)
	{
		fread(&blue, 1, 1, file);
		fread(&green, 1, 1, file);
		fread(&red, 1, 1, file);
		return (new_rgb(red, green, blue));
	}
	else if (header->bpp == 8)
	{
		fread(&red, 1, 1, file);
		return (palette[red]);
	}
	return (color2rgb(ERROR_CYAN));
}

void	pixel_read(FILE *file, t_bmp_header *header, int p[256], bmp_read *r)
{
	int		i;
	int		j;

	fseek(file, header->data_offset, SEEK_SET);
	i = r->height - 1;
	while (i >= 0)
	{
		j = -1;
		while (++j < r->widht)
		{
			(r->pixels)[i][j] = read_rgb(file, header, p);
		}
		fseek(file, (4 - (r->widht * 3) % 4) % 4, SEEK_CUR);
		i--;
	}
}
