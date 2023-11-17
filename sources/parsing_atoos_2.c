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

#define MAX_IMPORT_TEXTURES 100

const char	*g_imported_textures_names[MAX_IMPORT_TEXTURES] = {};
texture		g_imported_textures[MAX_IMPORT_TEXTURES] = {};
int			g_it_index = 0;

#define MAX_IMPORT_MATERIALS 100

const char	*g_imported_materials_names[MAX_IMPORT_MATERIALS] = {};
material	g_imported_materials[MAX_IMPORT_MATERIALS] = {};
int			g_im_index = 0;

bool	get_id(const char *tag, const char **dictionary, int *id)
{
	*id = 0;
	while (dictionary[*id] != NULL)
	{
		if (ft_strncmp(tag, dictionary[*id],
				max(len(tag), len(dictionary[*id]))) == 0)
			return (True);
		(*id)++;
	}
	*id = -1;
	return (False);
}

texture	atotexture(const char *str, texture __default)
{
	char	**check_split;
	int		id;

	if (str == NULL || ft_strncmp(str, "-", 2) == 0)
		return (__default);
	if (len(str) > 4 && ft_strncmp(str + len(str) - 4, ".bmp", 4) == 0)
		return (from_bmp(str));
	check_split = _gc_add_2d(ft_split_sep(str, "()|"));
	if (check_split && check_split[0]
		&& ft_strncmp(check_split[0], "CHECKERBOARD", 13) == 0)
	{
		return (checkerboard(atod(check_split[3], .5),
				atotexture(check_split[1], solid_color(ERROR_CYAN)),
				atotexture(check_split[2], solid_color(ERROR_CYAN))));
	}
	if (get_id(str, g_imported_textures_names, &id))
		return (g_imported_textures[id]);
	return (solid_color(atov3(str, ERROR_CYAN)));
}

material	atomaterial(const char **line, material __default)
{
	material	new;
	int			id;

	if (line == NULL || line[0] == NULL || ft_strncmp(line[0], "-", 2) == 0)
		return (__default);
	if (get_id(line[0], g_imported_materials_names, &id))
		return (g_imported_materials[id]);
	new.base_color = atotexture(line[0], WHITE_MAP);
	new.metalic = atotexture(line[1], NO_MAP);
	new.specular = atotexture(line[2], NO_MAP);
	new.specular_tint = atod(line[3], 0.0);
	new.roughness = atotexture(line[4], NO_MAP);
	new.ior = atod(line[5], 1.4);
	new.transmission = atotexture(line[6], NO_MAP);
	new.transmission_roughness = atotexture(line[7], NO_MAP);
	new.emission = atotexture(line[8], NO_MAP);
	new.emission_strength = atod(line[9], 0.0);
	new.normal = atotexture(line[10], c3(0.5, 0.5, 1.0));
	new.normal_strength = atov3(line[11], v3(1, 1, 1));
	new.normal_mode = atod(line[12], OPENGL);
	new.alpha = atotexture(line[13], FULL_MAP);
	return (new);
}

int	parse_texture(const char **line)
{
	if (g_it_index >= 99)
		return (dprintf(2, "Parse error: exceeded max \
		import textures limit of %d\n", MAX_IMPORT_TEXTURES), -1);
	if (ft_size(line) < 2)
		return (dprintf(2, "Texture parse error :: missing arguments\n"), -1);
	g_imported_textures_names[g_it_index] = line[0];
	g_imported_textures[g_it_index]
		= atotexture(line[1], solid_color(ERROR_CYAN));
	g_it_index++;
	return (1);
}

int	parse_material(const char **line)
{
	if (g_im_index >= 99)
		return (dprintf(2, "Parse error: exceeded max \
		import materials limit of %d\n", MAX_IMPORT_MATERIALS), -1);
	if (ft_size(line) < 2)
		return (dprintf(2, "Material parse error :: missing arguments\n"), -1);
	g_imported_materials_names[g_im_index] = line[0];
	g_imported_materials[g_im_index]
		= atomaterial(&(line[1]), new_m(ERROR_CYAN));
	g_im_index++;
	return (1);
}
