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

static inline void	signature_fail(const char *filename)
{
	write(1, "\33[2K\r", ft_strlen("\33[2K\r"));
	fprintf(stderr, ERROR" %s: File is not a BMP file\n", filename);
}

static inline void	headsize_12(FILE *file, t_bmp_header *header)
{
	int16_t	temp_width;
	int16_t	temp_height;

	fprintf(stderr, "READING headsize: 12..");
	fread(&temp_width, sizeof(int16_t), 1, file);
	fread(&temp_height, sizeof(int16_t), 1, file);
	fread(&header->planes, sizeof(uint16_t), 1, file);
	fread(&header->bpp, sizeof(uint16_t), 1, file);
	header->width = temp_width;
	header->height = temp_height;
}

static inline void	headsize_40(FILE *file, t_bmp_header *header)
{
	fprintf(stderr, "READING headsize: 40..");
	fread(&header->width, sizeof(int32_t), 1, file);
	fread(&header->height, sizeof(int32_t), 1, file);
	fread(&header->planes, sizeof(uint16_t), 1, file);
	fread(&header->bpp, sizeof(uint16_t), 1, file);
}

static inline void	headsize_less_128(FILE *file, t_bmp_header *header)
{
	if (header->header_size != 124)
		fprintf(stderr, WARNING" header size %d has not been tested\n",
			header->header_size);
	fprintf(stderr, "READING headsize: %d..", header->header_size);
	fread(&header->width, sizeof(int32_t), 1, file);
	fread(&header->height, sizeof(int32_t), 1, file);
	fread(&header->planes, sizeof(uint16_t), 1, file);
	fread(&header->bpp, sizeof(uint16_t), 1, file);
}

bool	read_header(FILE *file, t_bmp_header *header, const char *filename)
{
	fread(&header->signature, sizeof(uint16_t), 1, file);
	fread(&header->file_size, sizeof(uint32_t), 1, file);
	fread(&header->reserved1, sizeof(uint16_t), 1, file);
	fread(&header->reserved2, sizeof(uint16_t), 1, file);
	fread(&header->data_offset, sizeof(uint32_t), 1, file);
	fread(&header->header_size, sizeof(uint32_t), 1, file);
	if (header->signature != 0x4D42)
		return (signature_fail(filename), False);
	if (header->header_size == 12)
		headsize_12(file, header);
	else if (header->header_size == 40)
		headsize_40(file, header);
	else if (header->header_size <= 124)
		headsize_less_128(file, header);
	else
	{
		write(1, "\33[2K\r", ft_strlen("\33[2K\r"));
		fprintf(stderr, ERROR" %s: wrong HEADER size %u\n", filename,
			header->header_size);
		return (False);
	}
	return (True);
}
