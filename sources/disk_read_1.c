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

static inline void	close_file(FILE *file, const char *filename)
{
	fclose(file);
	write(1, "\33[2K\r", ft_strlen("\33[2K\r"));
	fprintf(stderr, ANSI_GREEN"READ"ANSI_RESET" %s: done!\n", filename);
}

// 0 on failure
// Function to read a BMP image file and return its data
bool	read_bmp(const char *filename, bmp_read *r)
{
	FILE			*file;
	t_bmp_header	header;
	int				palette[256];

	file = fopen(filename, "rb");
	if (!file)
		return (fprintf(stderr, ERROR" Unable to open file %s\n", filename), 0);
	if (!read_header(file, &header, filename))
		return (fclose(file), False);
	if (!read_palette(file, &header, palette))
		return (fclose(file), False);
	init_pixel_read(&header, r);
	pixel_read(file, &header, palette, r);
	close_file(file, filename);
	return (1);
}

bool	read_xmp(const char *filename, bmp_read *r)
{
	t__img 			new;
	int				x;
	int				y;
	t_bmp_header	fake_header;

	new.mlx_img = mlx_xpm_file_to_image(v.mlx, (char *)filename, &fake_header.width, &fake_header.height);
	if (new.mlx_img == NULL)
		return (dprintf(2, ERROR" MLX Couldn't read %s\n", filename), 0);
	new.addr = mlx_get_data_addr(new.mlx_img, &new.bpp, &new.line_len, &new.endian);
	init_pixel_read(&fake_header, r);
	y = 0;
	while (y < r->height)
	{
		x = 0;
		while (x < r->widht)
		{
			r->pixels[y][x] = get_pixel_(x, y, &new);
			x++;
		}
		y++;
	}
	mlx_destroy_image(v.mlx, new.mlx_img);
	return (True);
}

bool	read_png(const char *filename, bmp_read *r)
{
	return (False);
	// t__img *new;
	// int		x;
	// int		y;

	// new = mlx_png_file_to_image(v.mlx, (char *)filename, &(r->widht), &(r->height));
	// if (new == NULL)
	// 	return (False);
	// x = 0;
	// while (x < r->widht)
	// {
	// 	y = 0;
	// 	while (y < r->height)
	// 	{
	// 		r->pixels[y][x] = get_pixel_(x, y, new);
	// 		y++;
	// 	}
	// 	x++;
	// }
	// return (True);
}
