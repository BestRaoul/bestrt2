/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upate_item.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int		g_line = 1;

int		(*g_parsing_functions[30])(const char **) = {
	parse_ibl,
	parse_bg,
	parse_ambient_light,
	parse_point_lamp,
	parse_point_lamp,
	parse_directional_lamp,
	parse_camera,
	parse_camera_extension,
	parse_render_settings,
	parse_animation_settings,
	parse_sphere,
	parse_plane,
	parse_quad,
	parse_ss_quad,
	parse_box,
	parse_cylinder,
	parse_cone,
	parse_texture,
	parse_material,
	NULL};

char	*g_parsing_ids[30] = {
	"IBL",
	"BG",
	"A",
	"L",
	"LP",
	"LD",
	"C",
	"CE",
	"RS",
	"AS",
	"SP",
	"PL",
	"Q",
	"SQ",
	"BX",
	"CY",
	"CN",
	"T",
	"M",
	NULL};

int	get_idd(char *str)
{
	char	*_str;
	int		i;

	i = 0;
	while (str[i])
	{
		str[i] = ft_toupper(str[i]);
		i++;
	}
	i = 0;
	while (g_parsing_ids[i])
	{
		if (ft_strncmp(str, g_parsing_ids[i],
				max(ft_strlen(g_parsing_ids[i]), ft_strlen(str))) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	**get_split(const char *line)
{
	char	**line_split;
	char	**temp_split;
	int		count;

	temp_split = _gc_add_2d(ft_split_sep(line, " \t\n\v\f\r"));
	count = 0;
	while (temp_split[count++])
		;
	line_split = gc_malloc(sizeof(char *) * 101);
	memset(line_split, 0, 101 * sizeof(char *));
	memcpy(line_split, temp_split, count * sizeof(char *));
	return (line_split);
}

//1  :: Succcess
//0  :: parse failure (self prints)
//-1 :: Read error (crash)
int	parse_line(const char *line)
{
	char	**line_split;
	int		id;
	int		count;

	if (!line || len(line) < 2)
		return (1);
	line_split = get_split(line);
	if (!line_split || !line_split[0])
		return (1);
	id = get_idd(line_split[0]);
	if (id == -1)
		return (dprintf(2, WARNING" \
Unknown identifier [%s]\n", line_split[0]), 0);
	return (g_parsing_functions[id]((const char **)&(line_split[1])));
}

int	parse_file(const char *filename)
{
	char	*line;
	int		fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return ((void)perror(filename), -1);
	default_cam();
	line = get_next_line(fd);
	garbage_collector(ADD, line);
	while (line)
	{
		if (parse_line(line) == -1)
			return (close(fd), -1);
		line = get_next_line(fd);
		garbage_collector(ADD, line);
		g_line++;
	}
	if (close(fd) == -1)
		return ((void)perror(filename), -1);
	return (0);
}

//crash on its own
//rework with smart flag system
void	parse_2nd_argument(int ac, char **av)
{
	int	i;

	if (ac < 2)
		return (init_scene(100));
	i = 1;
	if (ft_strncmp(av[1], "-headless", 10) == 0)
	{
		(v.is_headless_print = True, i++);
		if (av[i] == NULL)
			(dprintf(2, ERROR" no scenes provided\n"), my_exit());
	}
	if (ft_strncmp(av[i], "-n", 3) == 0)
	{
		if (av[i + 1])
			init_scene(ft_atoi(av[i + 1]));
		else
			init_scene(-1);
	}
	else if (ft_strncmp(av[i], "-h", 3) == 0 || ft_strncmp(av[i], "-help", 3) == 0)
	{
		print_rt_format();
		my_exit();
	}
	else
	{
		populate_defaults();
		while (i < ac)
		{
			if (parse_file(av[i]) == -1)
				(dprintf(2, ERROR" in file %s\n", av[i]), my_exit());
			i++;
		}
	}
}
