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

int	parse_cylinder(const char **line)
{
	t_item	new;

	new.transform = (tfm){v3(), v3(), v_3(1)};
	new.hit = hit_cylinder;
	new.raster = raster_cylinder;
	new.transform.pos = atov3(*(line++), v3());
	new.transform.rot = atov3(*(line++), v3());
	new.transform.scale = atov3(*(line++), v_3(1));
	new.mat = atomaterial(line, new_m(random_v3()));
	add_item_(new);
	return (1);
}

int	parse_cone(const char **line)
{
	t_item	new;

	new.transform = (tfm){v3(), v3(), v_3(1)};
	new.hit = hit_cone;
	new.raster = raster_pyramid;
	new.transform.pos = atov3(*(line++), v3());
	new.transform.rot = atov3(*(line++), v3());
	new.transform.scale = atov3(*(line++), v_3(1));
	new.mat = atomaterial(line, new_m(random_v3()));
	add_item_(new);
	return (1);
}

void	print_rt_format_2(void);

void	print_rt_format(void)
{
	printf("["ANSI_RED".RT FORMAT"ANSI_RESET"]\n");
	printf("\n["ANSI_YELLOW"Vector 3"ANSI_RESET"]\n \"x,y,z\" or \"n\" = v3(n,n,n)\n");
	printf("\n["ANSI_YELLOW"Color"ANSI_RESET"]\n \"r,g,b\" in range [0.0, 1.0]\n");
	printf("\n["ANSI_YELLOW"Textures"ANSI_RESET"]\n \"(Color)\" \
which is gonna be a simple color\n");
	printf(" \"folder/file.bmp\" load from bmp\n");
	printf(" \"CHECKERBOARD((Texutre1)|(Texture2)|scale)\" \
checkerbaord pattern\n");
	printf(" \"texture name\" reuse previous texture\n");
	printf("\n\n");
	printf(" -- "ANSI_GREEN"TAGS"ANSI_RESET" -- \n");
	printf(" T	:: texture :: name [Texture]\n");
	printf(" M	:: material :: name albedo  metalic  specular  specular_tint  \
roughness  ior  transmission  transmission_roughness  emission  \
emission_strnegth  normal_map  normal_strength  \
normal_mode(0=OPENGL,1=DIRECTX)  alpha\n\n");
	print_rt_format_2();
}

void	print_rt_format_2(void)
{
	printf(" IBL	:: image based lighting :: irradiance_map\n");
	printf(" BG	:: background :: background\n");
	printf(" A	:: ambient lighting :: intensity  color\n");
	printf(" L / LP	:: point light :: position  intensity color\n");
	printf(" LD	:: directional light :: direction  inensity  color\n");
	printf(" C	:: camera :: position direction vfov\n");
	printf(" CE	:: camera extension :: lookat_toggle  lookat_position  \
defocus_angle  focus_distance  uv_debug_map  solo_lighting_toggle\n");
	printf(" RS	:: render settings :: width  height  upscale_ratio  max_depth  \
max_sample  samples_per_step\n");
	printf(" AS	:: animation settings :: duration  framerate  speed  \
render_mode  loops\n\n");
	printf("\n["ANSI_BLUE"Primitive"ANSI_RESET"]\n \
position  rotation  scale  material\n");
	printf(" SP	:: sphere\n");
	printf(" PL	:: plane\n");
	printf(" Q	:: quad\n");
	printf(" SQ	:: single side quad\n");
	printf(" BX	:: box\n");
	printf(" CY	:: cylinder\n");
	printf(" CN	:: cone\n");
}
