/*                                                        :::      ::::::::   */
/*   disk.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

# include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

const char *ff = "\%03d.bmp";
const char *outfolder = "output";

void writeBMP(const char *filename, int width, int height, Color *pixels);

void write_img(void)
{
    static int count = 0;


    int dir_fd = open(outfolder, __O_DIRECTORY);
    if (dir_fd == -1)
    {
        printf("no such directory \"%s\"\n", outfolder);
        return;
    }

    char filename[100];
    sprintf(filename, "%s/%s", outfolder, ff);
    sprintf(filename, filename, count);

    Color *pixels = malloc(v.w * v.h * sizeof(Color));
    
    for (int y=0; y<v.h; y++)
        for (int x=0; x<v.w; x++)
        {
            color c = rgb2color(get_pixel(x, v.h-y));
            pixels[y*v.w+x] = (Color){c.x * 255,
                                      c.y * 255,
                                      c.z * 255};
        }

    write(1, "writing img..", 14);
    writeBMP(filename, v.w, v.h, pixels);
    write(1, "done!", 5);

    free(pixels);
    count++;
}

char* getCurrentDateTime() {
    // Get the current time
    time_t currentTime;
    struct tm* timeInfo;
    char* dateTimeString = (char*)malloc(20); // Format: "YYYY-MM-DD HH:MM\0"

    if (dateTimeString == NULL) {
        perror("Memory allocation error");
        exit(1);
    }

    time(&currentTime);
    timeInfo = localtime(&currentTime);

    // Format the date and time as a string
    snprintf(dateTimeString, 20, "%04d_%02d_%02d_%02d_%02d",
             timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,
             timeInfo->tm_hour, timeInfo->tm_min);

    return dateTimeString;
}

#include <unistd.h>
//"\%03d.bmp"
/*
ffmpeg -i output/%03d.bmp -r 24 -c:v libx264 -preset slow -crf 18 output.mp4
*/
void ffmpeg_bmp_to_mp4(int framerate, int loops)
{
    char output_filename[100]; 
    char filename_format[100]; sprintf(filename_format, "%s/%s", outfolder, ff);

    sprintf(output_filename, "%s/render_%s.mp4", outfolder, getCurrentDateTime());

    char *argv[20] = {"/bin/ffmpeg",
        "-stream_loop", ft_itoa(loops),
        "-i", filename_format,
        "-r", ft_itoa(framerate),
        "-c:v", "libx264",
        "-preset", "slow",
        "-crf", "18",
        output_filename,
        NULL};
    execve(argv[0], argv, __environ);
}

// Function to write a BMP image
void writeBMP(const char *filename, int width, int height, Color *pixels) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    // BMP header
    uint8_t bmp_header[54] = {
        'B', 'M',           // BMP file signature
        0, 0, 0, 0,         // File size (placeholder)
        0, 0, 0, 0,         // Reserved
        54, 0, 0, 0,        // Pixel data offset

        40, 0, 0, 0,        // Header size
        0, 0, 0, 0,         // Image width (placeholder)
        0, 0, 0, 0,         // Image height (placeholder)
        1, 0,               // Number of color planes
        24, 0,              // Bits per pixel (24-bit RGB)
        0, 0, 0, 0,         // Compression method (none)
        0, 0, 0, 0,         // Image size (placeholder)
        0, 0, 0, 0,         // Horizontal resolution (pixels per meter)
        0, 0, 0, 0,         // Vertical resolution (pixels per meter)
        0, 0, 0, 0,         // Number of colors in palette
        0, 0, 0, 0,         // Important colors
    };

    // Calculate file size and image size
    uint32_t file_size = 54 + (width * height * 3);
    uint32_t image_size = width * height * 3;
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
    for (int i = 0; i < width * height; i++) {
        uint8_t b = pixels[i].b;
        uint8_t g = pixels[i].g;
        uint8_t r = pixels[i].r;
        fwrite(&b, sizeof(uint8_t), 1, fp);
        fwrite(&g, sizeof(uint8_t), 1, fp);
        fwrite(&r, sizeof(uint8_t), 1, fp);
    }

    // Close the file
    fclose(fp);
}

