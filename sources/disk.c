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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NTFY_SERVER "0.0.0.0:80/balls"
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
    pid_t id = fork(); //-1
    if (!id) execve(argv[0], argv, __environ);
    
    char attachement[120];
    sprintf(attachement, "Attach: %s", output_filename);

    waitpid(id, 0, 0); //-1
    char *av[20]= {"/bin/curl" ,
        "-H", "Title: The Render has finished",
        "-H", "Tags: tada",
        "-H", "Priority: urgent",
        "-H", "Click: https://youtube.com",
        "-H", attachement,
        "-d", "",
        NTFY_SERVER,
    };
    execve(av[0], av, __environ);
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

// Function to read a BMP image file and return its data
int readBMP(const char* filename, bmp_read *r) {
    
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return 0;
    }

    uint16_t signature;   // "BM" (0x4D42)
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
    fread(&headerSize, sizeof(uint32_t), 1, file);

    // Check if the file is actually a BMP file
    if (signature != 0x4D42) {
        fprintf(stderr, "Error: File %s is not a BMP file\n", filename);
        fclose(file);
        return 0;
    }
    
    int32_t width;       // Image width
    int32_t height;      // Image height
    uint16_t planes;     // Color planes (1)
    uint16_t bitsPerPixel; // Bits per pixel (usually 24)
    /* UNUSED
    uint32_t compression;  // Compression method (usually 0)
    uint32_t imageSize;    // Image data size (bytes)
    int32_t xPixelsPerMeter; // X Pixels per meter (usually 0)
    int32_t yPixelsPerMeter; // Y Pixels per meter (usually 0)
    uint32_t totalColors;    // Total colors (usually 0)
    uint32_t importantColors; // Important colors (usually 0)
    */
    
    int bytes_till_pixels = dataOffset - 18;

    if (headerSize == 12)
    {
        fprintf(stderr, "headsize: 12\n");
        int16_t temp_width;
        int16_t temp_height;
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
        fprintf(stderr, "headsize: 40\n");

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
        fprintf(stderr, "Warning: yolo reading bmp file with weird head size %u\n", headerSize);
        fread(&width, sizeof(int32_t), 1, file);
        fread(&height, sizeof(int32_t), 1, file);
        fread(&planes, sizeof(uint16_t), 1, file);
        fread(&bitsPerPixel, sizeof(uint16_t), 1, file);

        bytes_till_pixels -= 12;
    }
    else
    {
        fprintf(stderr, "Error: %u wrong HEADER size\n", headerSize);
        return 0;
    }

    // Check if the BMP format is supported (usually 24 bits per pixel)
    if (bitsPerPixel != 24) {
        fprintf(stderr, "Error: Unsupported BMP format (bits per pixel != 24)\n");
        fclose(file);
        return 0;
    }

    //Read till pixel data start
    char _c;
    while (bytes_till_pixels-- > 0)
        fread(&_c, 1, 1, file);

    r->widht = width;
    r->height = height;

    // Allocate memory for pixel data
    r->pixels = (int**)malloc(sizeof(int*) * r->widht);
    for (int i = 0; i < r->height; i++) {
        (r->pixels)[i] = (int*)malloc(sizeof(int) * r->widht);// * 3); // 3 bytes per pixel
    }

    // Read pixel data
    for (int i = r->height - 1; i >= 0; i--) {
        for (int j = 0; j < r->widht; j++) {
            uint8_t blue, green, red;
            fread(&blue, 1, 1, file);
            fread(&green, 1, 1, file);
            fread(&red, 1, 1, file);

            (r->pixels)[i][j] = new_rgb(red, green, blue);
            //(r->pixels)[i][j * 3] = (int)red;
            //(r->pixels)[i][j * 3 + 1] = (int)green;
            //(r->pixels)[i][j * 3 + 2] = (int)blue;
        }
        // Skip any padding bytes
        fseek(file, (4 - (r->widht * 3) % 4) % 4, SEEK_CUR);
    }

    fclose(file);
    return 1;
}
