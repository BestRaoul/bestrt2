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

#define NTFY_SERVER "0.0.0.0:80/balls"

// Get the current time
// Format: "YYYY-MM-DD HH:MM\0"
char	*get_current_date_time(void)
{
	time_t		current_time;
	struct tm	*time_info;
	char		*date_time_string;

	date_time_string = (char *)gc_malloc(20);
	if (date_time_string == NULL)
	{
		perror("Memory allocation error");
		exit(1);
	}
	time(&current_time);
	time_info = localtime(&current_time);
	snprintf(date_time_string, 20, "%04d_%02d_%02d_%02d_%02d",
		time_info->tm_year + 1900,
		time_info->tm_mon + 1,
		time_info->tm_mday,
		time_info->tm_hour,
		time_info->tm_min);
	return (date_time_string);
}

#ifdef ILLEGAL

void	ntfy_file(const char *filename, const char *output_filename,
	pid_t id, char *message)
{
	char	attachement[120];
	char	*av[20];

	sprintf(attachement, "Attach: %s", output_filename);
	waitpid(id, 0, 0);
	av[0] = "/bin/curl";
	av[1] = "-H";
	av[2] = "Title: The Render has finished";
	av[3] = "-H";
	av[4] = "Tags: tada";
	av[5] = "-H";
	av[6] = "Priority: urgent";
	av[7] = "-H";
	av[8] = "Click: https://youtube.com";
	av[9] = "-H";
	av[10] = attachement;
	av[11] = "-d";
	av[12] = "";
	av[13] = NTFY_SERVER;
	av[14] = NULL;
	execve(av[0], av, __environ);
}
#else

void	ntfy_file(const char *filename, const char *output_filename,
	pid_t id, char *message)
{
	notify_illegal(__func__);
	(void) filename;
	(void) output_filename;
	(void) id;
	(void) message;
}

#endif
//"\%03d.bmp"
/*
ffmpeg -i output/%03d.bmp -r 24 -c:v libx264 -preset slow -crf 18 output.mp4
*/
#ifdef ILLEGAL

void	ffmpeg_bmp_to_mp4(int framerate, int loops)
{
	char	output_filename[100];
	char	filename_format[100];
	char	*av[20];
	pid_t	id;

	av[0] = "/bin/ffmpeg";
	av[1] = "-stream_loop";
	av[2] = ft_itoa(loops);
	av[3] = "-i";
	av[4] = filename_format;
	av[5] = "-r";
	av[6] = ft_itoa(framerate);
	av[7] = "-c:v";
	av[8] = "libvpx-vp9";
	av[9] = "-crf";
	av[10] = "18";
	av[11] = output_filename;
	av[12] = NULL;
	sprintf(filename_format, "%s/%s", OUTFOLDER, FF);
	sprintf(output_filename, "%s/render_%s.webm", OUTFOLDER,
		get_current_date_time());
	id = fork();
	if (!id)
		execve(av[0], av, __environ);
	ntfy_file(output_filename, output_filename, id, "The Render has finished!");
}
#else

void	ffmpeg_bmp_to_mp4(int framerate, int loops)
{
	notify_illegal(__func__);
	(void) framerate;
	(void) loops;
}
#endif
