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
	fprintf(stderr, "READ %s: done!\n", filename);
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
		return (fprintf(stderr, "Eror: Unable to open file %s\n", filename), 0);
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
	return (False);
}
