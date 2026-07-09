
#include "fractol.h"

static inline void	close_file(FILE *file, const char *filename)
{
	fclose(file);
	write(1, "\33[2K\r", ft_strlen("\33[2K\r"));
	fprintf(stderr, ANSI_GREEN""ANSI_BOLD"READ"ANSI_RESET" %s: done!\n", filename);
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
	dprintf(2, ANSI_GREEN""ANSI_BOLD"READ"ANSI_RESET" %s: done!\n", filename);
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

bool	read_palette(FILE *file, t_bmp_header *header, int palette[256])
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
			write(1, "\33[2K\r", ft_strlen("\33[2K\r"));
			dprintf(2, ERROR" 8-bit bmp read from palette is not implemented\n");
			return (False);
		}
	}
	else
	{
		fprintf(stderr, ERROR" Unsupported BMP format \
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
	r->pixels = (int **)gc_malloc(sizeof(int *) * r->height);
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