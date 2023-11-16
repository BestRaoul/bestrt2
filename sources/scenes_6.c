/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

#ifdef ILLEGAL


void	specular(void)
{
	cornell(v3(0));
	v.render_mode = RAYTRACE_STEPS;

	material sph_1	   = new_lambertian(WHITE_MAP);
	material sph_04	   = new_lambertian(WHITE_MAP);
	material sph_015   = new_lambertian(WHITE_MAP);
	material sph_002   = new_lambertian(WHITE_MAP);

	sph_1.specular = BW_MAP(1.0);
	sph_04.specular = BW_MAP(0.4);
	sph_015.specular = BW_MAP(0.15);
	sph_002.specular = BW_MAP(0.02);

	sph_1.roughness = BW_MAP(0);
	sph_04.roughness = BW_MAP(0);
	sph_015.roughness = BW_MAP(0);
	sph_002.roughness = BW_MAP(0);

	v.max_depth = 10;
	v.upscale = 3;

	add_item(v3( 1.35, 0, 0),	v_3(.4),	v3(), 	sph_1,	SPHERE);
	add_item(v3(  .45, 0, 0),	v_3(.4),	v3(), 	sph_04,	SPHERE);
	add_item(v3( -.45, 0, 0),	v_3(.4),	v3(), 	sph_015,	SPHERE);
	add_item(v3(-1.35, 0, 0),	v_3(.4),	v3(), 	sph_002,	SPHERE);
}

void	mirrors(void)
{
	default_cam();
    v.render_mode = RAYTRACE;
	v.camera_pos = v3(-0.5003, 0.4719, 2.0912);//(-1.211402, 0.165707, 2.180947);
	v.camera_rot = v3(0.1090, 2.9723, 0.0000);
	v.background = cc3(BLACK);
	// v.lookat_toggle = 1;
	v.lookat = v3(0, 0, 0);
	v.upscale = 2;
	v.max_depth = 50;
	v.w = 60 * 16.0; //240 for 4k
	v.h = 60 * 9.0;
	v.upscale = 1;

	v.max_samples = v.samples_per_step = 1;

	PFPN fuzzzz = 0.0012;

    material material_wall = new_metal(c3(.73, .73, .73), fuzzzz);
	
	material material_wall_left  = new_metal(c3(.12, .45, .15), fuzzzz);
	material material_wall_bot   = new_metal(checkerboard(.1, c3(.1, .2, .8), c3(.3, .4, .8)), fuzzzz);
	material material_wall_right = new_metal(c3(.65, .05, .05), fuzzzz);

	PFPN _w = 16./9.;
	PFPN _h = 1;
	PFPN _d = 1;
	
	PFPN s = -0.01;

	add_item(v3( 0, _h+s, 0),	v3(_w, 1, _d),	v3(MYPI,0,0), 	material_wall,		SS_QUAD);
	add_item(v3( 0, 0,-_d-s),	v3(_w, 1, _h),	v3(MYPI/2,0,0), material_wall,		SS_QUAD);
	add_item(v3( 0, 0,_d+s),	v3(_w, 1, _h),	v3(MYPI/2,MYPI,0),material_wall,		SS_QUAD);
	
	add_item(v3( _w+s, 0, 0),	v3(_h, 1, _d),	v3(MYPI/2,-MYPI/2),	material_wall_left,	SS_QUAD);
	add_item(v3( 0,-_h-s, 0),	v3(_w, 1, _d),	v3(0,0,0), 		material_wall_bot, 	SS_QUAD);
	add_item(v3(-_w-s, 0, 0),	v3(_h, 1, _d),	v3(MYPI/2,MYPI/2),material_wall_right,SS_QUAD);

	material light = new_light(WHITE_MAP, 3.0);//checkerboard(0.2, c3(.1,.1,.1), c3(3,3,3)));
	light.metalic = BW_MAP(0.0);
	add_item(v3( 0, _h+s-1e-7, 0),	v_3(.8),	v3(MYPI,0,0), 	light,	SS_QUAD);

	material mirror = new_metal(WHITE_MAP, fuzzzz);
	add_item(v3( 0, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE);
}

void	tomato(void)
{
	cornell(v3());
	v.camera_pos = v3 (0.000000, -0.080084, 2.572990);
	v.lookat_toggle = 1;
	v.max_depth = 3;
	v.render_mode = RAYTRACE_STEPS;


	v.items[0].mat = new_lambertian(c3(.4, .4, .4));
	v.items[1].mat = new_lambertian(c3(.2, .3, .8));
	v.items[3].mat = new_lambertian(WHITE_MAP);
	material bot = new_lambertian(c3(0.1, 0.1, 0.1));
	bot.specular = BW_MAP(0.1);
	bot.roughness = BW_MAP(0.3);
	v.items[4].mat = bot;
	v.items[5].mat = new_lambertian(WHITE_MAP);
	v.item_count = 6;

	material tom   = new_lambertian(c3(1.0, .1, .2));
	tom.specular = BW_MAP(0.2);
	tom.roughness = BW_MAP(0);

	add_item(v3( 0, -.3, 0),	v_3(.7),	v3(), 	tom,	SPHERE);

	material light = new_light(WHITE_MAP, 15.0);

	add_item(v3( 15.9/9.0, .7, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD);
	add_item(v3( 15.9/9.0,  0, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD);
	add_item(v3( 15.9/9.0,-.7, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD);
	add_item(v3(-15.9/9.0,  0, 0),	v_3(.4),	v3(0, 0, -MYPI/2), 	light,	SS_QUAD);

}

void	glass_ball(void)
{
	default_cam();
	v.camera_pos = v3 (2.436390, -0.080084, 0.978884);
	v.lookat = v3(0, -.25, 0);
	v.lookat_toggle = 1;
	v.max_depth = 8;
	v.upscale = 1;
	v.render_mode = RAYTRACE_STEPS;
	v.w = 400;
	v.h = 400;


	texture trn = checkerboard(0.5, c3(0,0,0), c3(1,1,1));
	texture col = checkerboard(0.5, c3(1,1,1), c3(.2,.8,.3));
	material glass   = new_dielectric(col, 1);
	glass.transmission_roughness = trn;
	add_item(v3( 0, -.3, 0),	v_3(.7),	v3(), 	glass,	SPHERE);

	material ground   = new_lambertian(checkerboard(.2, c3(.1, .2, .8), c3(.3, .4, .8)));
	add_item(v3( 0,-1, 0),	v3(1, 1, 1),	v3(0,0,0), 		ground, 	PLANE);

	material light = new_light(WHITE_MAP, 5.0);
	add_item(v3( 0, 1, 0),	v_3(.2),	v3(MYPI, 0), 	light,	SS_QUAD);
}

void	earth(void)
{
	default_cam();
	v.camera_pos = v3(0, 0, 2.5);
	v.w = 500;
	v.h = 500;
	v.upscale = 1;
	v.lookat_toggle = 1;
	v.background = cc3(WHITE);
	v.irradiance_map = WHITE_MAP;//from_bmp("textures/emptyroom1k.bmp");
	v.max_depth = 0;

	texture earth_texture = from_bmp("textures/earthmap1k.bmp");
	material earth = new_lambertian(earth_texture);
	earth.specular = NO_MAP;
	earth.roughness = NO_MAP;
	earth.normal = from_bmp("textures/earthnormal1k.bmp");
	earth.normal_strength = v3(-1,1,1);

	material ground = new_lambertian(checkerboard(.25, c3(.1,.1,.1), c3(.9,.9,.9)));
	ground.specular = NO_MAP;
	ground.roughness = NO_MAP;

	add_item(v3( 0, 0, 0),	v_3(1),	v3(0,MYPI/4), 	earth,	SPHERE);
	add_item(v3( 0, -1, 0),v3(1,1,1),	v3(), 	ground,	PLANE);

	add_lamp(WHITE, v3(-1,-1,-1), 2.0, True);
	
	v.max_depth = 1;
	v.max_samples = 1;
	v.samples_per_step = 1;
}
#endif
