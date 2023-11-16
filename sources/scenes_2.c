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

//!1 : green plane, 3 balls (gold, blue, hollow glass)
//ok, 1 noise
void	showoff_1(void)
{
	default_cam();
	v.w = 960; v.h = 540;
    v.render_mode = RAYTRACE;
	v.lookat_toggle = 1;
	v.lookat = v3(0,0,0);
	v.camera_pos = v3(0.850230, 0.779302, 2.851645);
	v.vfov = 50;
	v.ambient = 0;
	v.background = cc3(WHITE);
	v.irradiance_map = NO_MAP;
	v.max_samples = 10;
	v.max_depth = 3;
	v.upscale = 1;

    material material_ground = new_lambertian(checkerboard(.5, c3(.2,.3,.1), c3(.9, .9, .9)));
    material material_center = new_lambertian(c3(0.1, 0.2, 0.4));
    material material_left	 = new_dielectric(c3(1,1,1), 1.5);
	material_left.transmission = FULL_MAP;
    material material_right  = new_metal(c3(0.8, 0.6, 0.2), 0.02);

    add_item(v3( 0, -.5, 0),	v_3(1),	v3(), material_ground, PLANE);
    
    add_item(v3( 0, 0, 0),		v_3(0.5),	v3(), material_center, SPHERE);
	add_item(v3(-1, 0, 0),		v_3(0.5),	v3(), material_left, SPHERE);
	//add_item(v3(-1, 0, 0),		v_3(-0.4),	v3(), material_left, SPHERE);
	add_item(v3( 1, 0, 0),		v_3(0.5),	v3(), material_right, SPHERE);
}

//!2 : ball 3 point lights, checkplane
//good, 1 noise
void	showoff_2(void)
{
	default_cam();
	v.background = cc3(BLACK);
	v.irradiance_map = NO_MAP;
	v.lookat = v3(0, 1, 0);
	v.lookat_toggle = 1;
	v.camera_pos = v3(0, 1.0, 3.5);
	v.upscale = 3;
	v.samples_per_step = 1;
	
	material check = new_lambertian(checkerboard(.5,
		c3(.1, .1, .1),
		c3(.9, .9, .9)));
	material sphere = new_lambertian(c3(1,1,1));

    add_item(v3( 0, 0, 0),	v_3(1),	v3(), check, PLANE);
    add_item(v3( 0, 1, 0),	v_3(1),	v3(), sphere, SPHERE);

	PFPN _h = 3;
	PFPN _s = 1;

	add_lamp(RED, v3(-_s,_h, _s), 5.0, False);
	add_lamp(GREEN, v3( _s,_h, _s), 5.0, False);
	add_lamp(BLUE, v3(-_s,_h,-_s), 5.0, False);
}

//3 : cornell, sphere (1 emissive light)
//meh, 3 noise
void	showoff_3(void)
{
	cornell(v3(1,1,1));
	v.w = 900; v.h = 900;
	v.upscale = 3;
	v.max_depth = 5;
	v.max_samples = 100;

	material ball = new_lambertian(WHITE_MAP);
	ball.metalic = FULL_MAP;
	add_item(v3( 0, 0, 0),	v_3(.5),	v3(), ball, SPHERE);

	material lit = new_lambertian(NO_MAP);
	lit.emission = WHITE_MAP;
	lit.emission_strength = 2.0;
	add_item(v3( 0, .99, 0),	v_3(.8),	v3(MYPI), lit, SS_QUAD);
}

//4 : cornell, tomato (4 emissive lights)
//bad, 5 noise
void	showoff_4(void)
{
	cornell(v3());
	v.max_depth = 3;
	v.max_samples = 30;

	v.items[0].mat = new_lambertian(c3(.4, .4, .4));
	v.items[1].mat = new_lambertian(c3(.2, .3, .8));
	v.items[3].mat = new_lambertian(WHITE_MAP);
	v.items[5].mat = new_lambertian(WHITE_MAP);
	
	material bot = new_lambertian(c3(0.1, 0.1, 0.1));
	bot.specular = BW_MAP(0.1);
	bot.roughness = BW_MAP(0.3);
	v.items[4].mat = bot;

	material tom   = new_lambertian(c3(1.0, .1, .2));
	tom.specular = BW_MAP(0.5);
	tom.roughness = BW_MAP(0);

	add_item(v3( 0, -.3, 0),	v_3(.7),	v3(), 	tom,	SPHERE);

	material light = new_light(WHITE_MAP, 15.0);

	add_item(v3( 15.9/9.0, .7, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD);
	add_item(v3( 15.9/9.0,  0, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD);
	add_item(v3( 15.9/9.0,-.7, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD);
	add_item(v3(-15.9/9.0,  0, 0),	v_3(.4),	v3(0, 0, -MYPI/2), 	light,	SS_QUAD);
}

//!5 : cornell, glass cylinder, mirror box
void	showoff_5(void)
{
	cornell(v3(1,1,1));
	v.w = v.h = 900;
	v.upscale = 3;
	v.max_depth = 6;
	v.max_samples = 10;
	v.samples_per_step = 2;
	v.render_mode = RAYTRACE_STEPS;

	material cylinder = new_lambertian(WHITE_MAP);
	cylinder.transmission = FULL_MAP; cylinder.ior = 1.1;
	add_item(v3(.5, -.299,-.4),	v3(.4,.7,.4),	v3(), cylinder, CYLINDER);
	
	material box = new_lambertian(WHITE_MAP);
	box.metalic = FULL_MAP;
	add_item(v3(-.5, -.7,.6),	v_3(.3),	v3(0,-MYPI/10), box, BOX);
	
	// material lit = new_light(WHITE_MAP, 2.0);
	// add_item(v3( 0, .99, 0),	v_3(.8),	v3(MYPI), lit, SS_QUAD);

	add_lamp(WHITE, v3(.9,.9,.9), 1.0, False);
}
#endif
