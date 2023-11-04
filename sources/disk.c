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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>

#define NTFY_SERVER "0.0.0.0:80/balls"

typedef struct
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
}   Color;

const char  *ff = "\%03d.bmp";
const char  *outfolder = "output";

void	writeBMP(const char *filename, int w, int h, Color *pixels);

void	write_img(void)
{
	static int	count = 0;
	int			dir_fd;
	char		filename[100];
	Color		*pixels;
	color		c;

	dir_fd = open(outfolder, __O_DIRECTORY);
	if (dir_fd == -1)
	{
		printf("no such directory \"%s\"\n", outfolder);
		return ;
	}
	sprintf(filename, "%s/%s", outfolder, ff);
	sprintf(filename, filename, count);
	pixels = gc_malloc(v.w * v.h * sizeof(Color));
	write(1, "writing img..", 14);
	for (int y = 0; y < v.h; y++)
		for (int x = 0; x < v.w; x++)
		{
			c = rgb2color(get_pixel(x, v.h - y));
			pixels[y * v.w + x] = (Color){c.x * 255, c.y * 255, c.z * 255};
		}
	writeBMP(filename, v.w, v.h, pixels);
	write(1, "done!", 5);
	gc_free(pixels);
	count++;
}

// Get the current time
// Format: "YYYY-MM-DD HH:MM\0"
char	*getCurrentDateTime(void)
{
	time_t      currentTime;
	struct tm	*timeInfo;

	char *dateTimeString = (char *)gc_malloc(20);
	if (dateTimeString == NULL)
	{
		perror("Memory allocation error");
		exit(1);
	}
	time(&currentTime);
	timeInfo = localtime(&currentTime);
	snprintf(dateTimeString, 20, "%04d_%02d_%02d_%02d_%02d", timeInfo->tm_year
		+ 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour,
		timeInfo->tm_min);
	return (dateTimeString);
}


//"\%03d.bmp"
/*
ffmpeg -i output/%03d.bmp -r 24 -c:v libx264 -preset slow -crf 18 output.mp4
*/
void	ffmpeg_bmp_to_mp4(int framerate, int loops)
{
	char	output_filename[100];
	char	filename_format[100];
	char	*argv[20] = {"/bin/ffmpeg", "-stream_loop", ft_itoa(loops), "-i",
			filename_format, "-r", ft_itoa(framerate), "-c:v", "libvpx-vp9",
			"-crf", "18", output_filename, NULL};
	char	attachement[120];

	sprintf(filename_format, "%s/%s", outfolder, ff);
	sprintf(output_filename, "%s/render_%s.webm", outfolder,
		getCurrentDateTime());
	pid_t id = fork(); //-1
	if (!id)
		execve(argv[0], argv, __environ);
	sprintf(attachement, "Attach: %s", output_filename);
	waitpid(id, 0, 0); //-1
	char *av[20] = {
		"/bin/curl",
		"-H",
		"Title: The Render has finished",
		"-H",
		"Tags: tada",
		"-H",
		"Priority: urgent",
		"-H",
		"Click: https://youtube.com",
		"-H",
		attachement,
		"-d",
		"",
		NTFY_SERVER,
	};
	execve(av[0], av, __environ);
}

// Function to write a BMP image
void	writeBMP(const char *filename, int width, int height, Color *pixels)
{
	FILE		*fp;
	uint32_t	file_size;
	uint32_t	image_size;
	uint8_t		b;
	uint8_t		g;
	uint8_t		r;

	fp = fopen(filename, "wb");
	if (!fp)
	{
		perror("Error opening file");
		return ;
	}
	// BMP header
	uint8_t bmp_header[54] = {
		'B', 'M',       // BMP file signature
		0,   0,   0, 0, // File size (placeholder)
		0,   0,   0, 0, // Reserved
		54,  0,   0, 0, // Pixel data offset
		40,  0,   0, 0, // Header size
		0,   0,   0, 0, // Image width (placeholder)
		0,   0,   0, 0, // Image height (placeholder)
		1,   0,         // Number of color planes
		24,  0,         // Bits per pixel (24-bit RGB)
		0,   0,   0, 0, // Compression method (none)
		0,   0,   0, 0, // Image size (placeholder)
		0,   0,   0, 0, // Horizontal resolution (pixels per meter)
		0,   0,   0, 0, // Vertical resolution (pixels per meter)
		0,   0,   0, 0, // Number of colors in palette
		0,   0,   0, 0, // Important colors
	};
	// Calculate file size and image size
	file_size = 54 + (width * height * 3);
	image_size = width * height * 3;
	// Fill in header data
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
	// Write BMP header
	fwrite(bmp_header, sizeof(uint8_t), 54, fp);
	// Write pixel data (BGR order)
	for (int i = 0; i < width * height; i++)
	{
		b = pixels[i].b;
		g = pixels[i].g;
		r = pixels[i].r;
		fwrite(&b, sizeof(uint8_t), 1, fp);
		fwrite(&g, sizeof(uint8_t), 1, fp);
		fwrite(&r, sizeof(uint8_t), 1, fp);
	}
	// Close the file
	fclose(fp);
}

// 0 on failure
// Function to read a BMP image file and return its data
int	readBMP(const char *filename, bmp_read *r)
{
	FILE	*file;
	int		bytes_till_pixels;
		int16_t temp_width;
		int16_t temp_height;
	int		palette[256];
	uint8_t bw;

	file = fopen(filename, "rb");
	if (!file)
	{
		fprintf(stderr, "Error: Unable to open file %s\n", filename);
		return (0);
	}
	uint16_t signature;  // "BM" (0x4D42)
	uint32_t fileSize;   // File size in bytes
	uint16_t reserved1;  // Reserved (0)
	uint16_t reserved2;  // Reserved (0)
	uint32_t dataOffset; // Pixel data offset
	uint32_t headerSize; // Header size (12)
	fread(&signature, sizeof(uint16_t), 1, file);
	fread(&fileSize, sizeof(uint32_t), 1, file);
	fread(&reserved1, sizeof(uint16_t), 1, file);
	fread(&reserved2, sizeof(uint16_t), 1, file);
	fread(&dataOffset, sizeof(uint32_t), 1, file);
	fread(&headerSize, sizeof(uint32_t), 1, file); // does not have to be here, pixels could already be here
	// Check if the file is actually a BMP file
	if (signature != 0x4D42)
	{
		write(1, "\33[2K\r", ft_strlen("\33[2K\r"));
		fprintf(stderr, "Error %s: File is not a BMP file\n", filename);
		fclose(file);
		return (0);
	}
	int32_t width;         // Image width
	int32_t height;        // Image height
	uint16_t planes;       // Color planes (1)
	uint16_t bitsPerPixel; // Bits per pixel (usually 24)
	/* UNUSED
	uint32_t compression;  // Compression method (usually 0)
	uint32_t imageSize;    // Image data size (bytes)
	int32_t xPixelsPerMeter; // X Pixels per meter (usually 0)
	int32_t yPixelsPerMeter; // Y Pixels per meter (usually 0)
	uint32_t totalColors;    // Total colors (usually 0)
	uint32_t importantColors; // Important colors (usually 0)
	*/
	bytes_till_pixels = dataOffset - 18;
	if (headerSize == 12)
	{
		fprintf(stderr, "READING headsize: 12..");
		fread(&temp_width, sizeof(int16_t), 1, file);
		fread(&temp_height, sizeof(int16_t), 1, file);
		fread(&planes, sizeof(uint16_t), 1, file);
		fread(&bitsPerPixel, sizeof(uint16_t), 1, file);
		width = temp_width;
		height = temp_height;
		bytes_till_pixels -= 8;
	}
	else if (headerSize == 40)
	{
		fprintf(stderr, "READING headsize: 40..");
		fread(&width, sizeof(int32_t), 1, file);
		fread(&height, sizeof(int32_t), 1, file);
		fread(&planes, sizeof(uint16_t), 1, file);
		fread(&bitsPerPixel, sizeof(uint16_t), 1, file);
		bytes_till_pixels -= 12;
		/* UNUSED
		fread(&compression, sizeof(uint32_t), 1, file);
		fread(&imageSize, sizeof(uint32_t), 1, file);
		fread(&xPixelsPerMeter, sizeof(int32_t), 1, file);
		fread(&yPixelsPerMeter, sizeof(int32_t), 1, file);
		fread(&totalColors, sizeof(uint32_t), 1, file);
		fread(&importantColors, sizeof(uint32_t), 1, file);
		*/
	}
	else if (headerSize <= 124)
	{
		if (headerSize != 124)
			fprintf(stderr, "WARNING: header size %d has not been tested\n",
				headerSize);
		fprintf(stderr, "READING headsize: %d..", headerSize);
		// fprintf(stderr, "Warning: yolo reading bmp file with weird head size %u\n", headerSize);
		fread(&width, sizeof(int32_t), 1, file);
		fread(&height, sizeof(int32_t), 1, file);
		fread(&planes, sizeof(uint16_t), 1, file);
		fread(&bitsPerPixel, sizeof(uint16_t), 1, file);
		// compression type (0=none, 1=RLE-8, 2=RLE-4)
		// size of image data in bytes (including padding)
		// horizontal resolution in pixels per meter (unreliable)
		// vertical resolution in pixels per meter (unreliable)
		// number of colors in image, or zero
		// number of important colors, or zero
		uint32_t compression_type, size_of_img_data, h_resolution, v_resolution,
			num_colors, num_important_colors;
		fread(&compression_type, sizeof(uint32_t), 1, file);
		fread(&size_of_img_data, sizeof(uint32_t), 1, file);
		fread(&h_resolution, sizeof(uint32_t), 1, file);
		fread(&v_resolution, sizeof(uint32_t), 1, file);
		fread(&num_colors, sizeof(uint32_t), 1, file);
		fread(&num_important_colors, sizeof(uint32_t), 1, file);
		// printf("compression type %d\n", compression_type);
		// printf("size %d\n", size_of_img_data);
		// printf("res %d x %d\n", h_resolution, v_resolution);
		// printf("N colors: %d / %d\n", num_colors, num_important_colors);
		bytes_till_pixels -= 12;
	}
	else
	{
		write(1, "\33[2K\r", ft_strlen("\33[2K\r"));
		fprintf(stderr, "Error %s: wrong HEADER size %u\n", filename,
			headerSize);
		fclose(file);
		return (0);
	}
	if (bitsPerPixel == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			uint8_t r, g, b, RESERVED;
			fread(&r, 1, 1, file);
			fread(&g, 1, 1, file);
			fread(&b, 1, 1, file);
			fread(&RESERVED, 1, 1, file);
			palette[i] = new_rgb(r, g, b);
			bytes_till_pixels -= 4;
		}
	}
	// printf("bytes left: %d\n", bytes_till_pixels);
	// Check if the BMP format is supported (usually 24 bits per pixel)
	if (bitsPerPixel == 1 || bitsPerPixel == 4)
	{
		NOT_IMPLEMENTED("bmp 1-bit and 4-bit support");
		fclose(file);
		return (0);
	}
	else if (bitsPerPixel == 8 || bitsPerPixel == 24)
	{
		if (bitsPerPixel == 8)
		{
			NOT_IMPLEMENTED("add 8-bit bmp read from palette");
			return (False);
		}
	}
	else
	{
		fprintf(stderr, "Error: Unsupported BMP format (bits per pixel %u != 24)\n", bitsPerPixel);
		fclose(file);
		return (0);
	}
	r->widht = width;
	r->height = height;
	// Allocate memory for pixel data
	r->pixels = (int **)gc_malloc(sizeof(int *) * r->widht);
	for (int i = 0; i < r->height; i++)
	{
		(r->pixels)[i] = (int *)gc_malloc(sizeof(int) * r->widht); // * 3);
			// 3 bytes per pixel
	}
	// Read pixel data
	fseek(file, dataOffset, SEEK_SET);
	for (int i = r->height - 1; i >= 0; i--)
	{
		for (int j = 0; j < r->widht; j++)
		{
			if (bitsPerPixel == 24)
			{
				uint8_t blue, green, red;
				fread(&blue, 1, 1, file);
				fread(&green, 1, 1, file);
				fread(&red, 1, 1, file);
				(r->pixels)[i][j] = new_rgb(red, green, blue);
			}
			else if (bitsPerPixel == 8)
			{
				fread(&bw, 1, 1, file);
				// sleep(1);
				(r->pixels)[i][j] = palette[bw];
			}
			else
				NOT_IMPLEMENTED("how did you even get here");
		}
		// Skip any padding bytes
		fseek(file, (4 - (r->widht * 3) % 4) % 4, SEEK_CUR);
	}
	fclose(file);
	write(1, "\33[2K\r", ft_strlen("\33[2K\r"));
	fprintf(stderr, "READ %s: done!\n", filename);
	return (1);
}

int	readXMP(const char *filename, bmp_read *r)
{
	return (False);
}
