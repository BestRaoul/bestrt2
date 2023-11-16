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

//!6 : earth in space normal mapped
void	showoff_6(void)
{
	default_cam();
	v.camera_pos = v3(0, 0, 2.5);
	v.lookat_toggle = 1;
	v.w = v.h = 500;
	v.background = NO_MAP;
	v.max_samples = 1;
	v.samples_per_step = 2;
	v.upscale = 1;
	v.max_depth = 1;

	texture earth_color = from_bmp("textures/earthmap1k.bmp");
	texture earth_norml = from_bmp("textures/earthnormal1k.bmp");
	material earth = new_lambertian(earth_color);
	earth.normal = earth_norml;
	earth.normal_strength = v3(-.5,.5,.5);
	earth.specular = BW_MAP(0.32);
	earth.roughness = BW_MAP(0.5);

	add_item(v3(), v3(1,1,1), v3(0), earth, SPHERE);

	add_lamp(vrgb(244, 233, 185), v3(2, 2, 2), 8.0, False);
	
	PFPN *ry = &(v.items[0].transform.rot.y);
	add_motion(ry, 0, 2*MYPI, lerpd);

	v.animation_duration = 2;
	v.animation_framerate = 24;
	v.animation_speed = .5;
	v.animation_loops = 1;
	v.animation_render_mode = RAYTRACE;
}

//!7 : box with rough surface, sphere with rust, cylinder watery, ...
void	showoff_7(void)
{
	default_cam();
	v.camera_pos = v3(-7.8285, 5.3976, 0.5486);
	v.lookat_toggle = 1;
	v.upscale = 1;
	v.background = NO_MAP;
	v.max_depth = 6;
	v.max_samples = 1;v.samples_per_step = 2;
	v.irradiance_map = from_bmp("textures/hdri/emptyroom1k.bmp");
	v.use_ibl = 1;
	v.use_background = 1;
	v.background = WHITE_MAP;
	
	//WOODEN TILE FLOOR
	material floor = new_lambertian(from_bmp("textures/wooden_floor/woodenmap.bmp"));
	floor.roughness = from_bmp("textures/wooden_floor/woodenrough.bmp");
	floor.normal = from_bmp("textures/wooden_floor/woodennormal.bmp");
	floor.specular = BW_MAP(0.2);

	//METAL BOX
	material box = new_lambertian(from_bmp("textures/metal_panel/color.bmp"));
	box.emission = from_bmp("textures/metal_panel/emissive.bmp"); box.emission_strength = 5;
	box.metalic = from_bmp("textures/metal_panel/metalic.bmp");
	box.normal = from_bmp("textures/metal_panel/normal.bmp");
	box.roughness = NO_MAP;//from_bmp("textures/metal_panel/rough.bmp");

	//METAL MIRROR BOX
	material box2 = new_lambertian(from_bmp("textures/metal_wall/color.bmp"));
	box2.metalic = from_bmp("textures/metal_wall/metalic.bmp");
	box2.normal = from_bmp("textures/metal_wall/normal.bmp");
	box2.roughness = NO_MAP;//from_bmp("textures/metal_wall/rough.bmp");

	//GATE sphere
	material sph = new_lambertian(from_bmp("textures/metalic_gate/color.bmp"));
	sph.metalic = from_bmp("textures/metalic_gate/metalic.bmp");
	sph.normal = from_bmp("textures/metalic_gate/normal.bmp");
	sph.transmission = from_bmp("textures/metalic_gate/transmission.bmp"); sph.ior = 1.0;

	//FABRIC sphere
	material sph2 = new_lambertian(from_bmp("textures/leather/color.bmp"));
	// sph2.metalic = from_bmp("textures/leather/metalic.bmp");
	sph2.normal = from_bmp("textures/leather/normal.bmp");
	sph2.roughness = from_bmp("textures/leather/rough.bmp");

	//WINDOW cylinder
	material cyl = new_lambertian(from_bmp("textures/wood_window/color.bmp"));
	cyl.roughness = from_bmp("textures/wood_window/rough.bmp");
	cyl.metalic = from_bmp("textures/wood_window/metalic.bmp");
	cyl.normal = from_bmp("textures/wood_window/normal.bmp");
	cyl.transmission = from_bmp("textures/wood_window/transmission.bmp"); cyl.ior = 1.0;

	//WOOD ctlinder
	material cyl2 = new_lambertian(from_bmp("textures/wood/color.bmp"));
	cyl2.roughness = from_bmp("textures/wood/rough.bmp");
	cyl2.normal = from_bmp("textures/wood/normal.bmp");


	PFPN s = 5;
	PFPN d0 = 5;
	PFPN d1 = 3;

    add_item(v3( 0, 0, 0),		v_3(6),	v3(), floor, PLANE);
    add_item(v3( 0, 1,-d0),	v_3(1),	v3(), box, BOX);
    add_item(v3(-s, 1,-d1),	v_3(1),	v3(), box2, BOX);
    add_item(v3(-s, 1, d1),	v_3(1),	v3(), sph, SPHERE);
    add_item(v3( s, 1, d1),	v_3(1),	v3(), sph2, SPHERE);
    add_item(v3( 0, 4, d0),	v3(2,4,2),	v3(0, MYPI/3), cyl, CYLINDER);
    add_item(v3( s, 1,-d1),	v3(1,1,1),	v3(), cyl2, CYLINDER);

//LIGHTS
	add_lamp(vrgb(244, 233, 185), v_norm(v3(-1, -1, -1)), .5, True);
}

//!8 : shapes roitatin
//clean and small noise
void	showoff_8(void)
{
	default_cam();
	v.upscale = 3;
	v.w = v.h = 500;
	v.camera_pos = v3(0,3,8);
	v.lookat_toggle = 1;
	v.max_samples = 1;
	v.samples_per_step = 2;
	v.background = cc3(vrgb(254, 250, 224));

	PFPN s = 1.5;

	add_item(v3( s,-s),		v_3(1),	v3(), new_m_rgb(v3(96, 108, 56)), BOX);
	add_item(v3( s, s),		v_3(1),	v3(), new_m_rgb(v3(40, 54, 24)), SPHERE);
	add_item(v3(-s,-s),		v_3(1),	v3(), new_m_rgb(v3(221, 161, 94)), CYLINDER);
	add_item(v3(-s, s),		v_3(1),	v3(), new_m_rgb(v3(188, 108, 37)), CONE);

	for (int i=0; i<4; i++)
	{
		add_motion(&(v.items[i].transform.rot.x), 0, 2*MYPI, lerpd);
		add_motion(&(v.items[i].transform.rot.y), 0, 2*MYPI, lerpd);
		add_motion(&(v.items[i].transform.rot.z), 0, 2*MYPI, lerpd);
	}
	v.animation_duration = 2;
	v.animation_speed = 0.5;
	v.animation_framerate = 30;
	v.animation_render_mode = RAYTRACE_UVS;
	v.animation_loops = 1;
}

//!9 : materials (rows of balls)
void	showoff_9(void)
{
	default_cam();
	// v.background = cc3(BLACK);
	v.max_samples = v.samples_per_step = 2;
	v.camera_pos = v3(6, -2.4, 90);
	v.w = 1100;	v.h = 500;
	v.vfov = 4;
	v.upscale = 1;
	v.max_depth = 0;
	v.irradiance_map = from_bmp("textures/hdri/emptyroom1k.bmp");
	v.use_ibl = 1;
	v.background = BLACK_MAP;
	v.solo_lighting = 1;

//DEFINE materials
	material row1_metalic = new_lambertian(c3(0.8, 0.6, 0.2));
	row1_metalic.roughness = BW_MAP(0.2);
	row1_metalic.specular = BW_MAP(0.32);
	material row2_specular = new_lambertian(c3(1,0,0));
	row2_specular.roughness = BW_MAP(0.2);
	material row3_roughness = new_lambertian(c3(.6,.6,.8));
	row3_roughness.specular = BW_MAP(1.0);

	material row4_transmission = new_lambertian(c3(139.0/256.0,173.0/256.0,158.0/256.0));
	material row5_t_roughness = new_lambertian(c3(139.0/256.0,173.0/256.0,158.0/256.0));
	row5_t_roughness.transmission = FULL_MAP;

	//for 1-10
	int count = 10;
	PFPN sh = 1.2;
	PFPN sv = 1.2;
	for (int i=0; i<=count; i++)
	{
		PFPN ratio = 1.0 - (PFPN)i/(PFPN)count;
		row1_metalic.metalic 					= BW_MAP(ratio);
		row2_specular.specular 					= BW_MAP(ratio);
		row3_roughness.roughness 				= BW_MAP(ratio);
		row4_transmission.transmission 			= BW_MAP(ratio);
		row5_t_roughness.transmission_roughness = BW_MAP(ratio);

		add_item(v3(sh*i, 0*sv),	v_3(.5), v3(), row1_metalic, SPHERE);
		add_item(v3(sh*i,-1*sv),	v_3(.5), v3(), row2_specular, SPHERE);
		add_item(v3(sh*i,-2*sv),	v_3(.5), v3(), row3_roughness, SPHERE);
		add_item(v3(sh*i,-3*sv),	v_3(.5), v3(), row4_transmission, SPHERE);
		add_item(v3(sh*i,-4*sv),	v_3(.5), v3(), row5_t_roughness, SPHERE);
	}

	add_lamp(WHITE, v_norm(v3(-1,-.5,-1)), 1.0, True);
	write(1, "1: Metalic\n2: Specular\n3: Roughness\n4: transmission\n5: Transmission roughness\n", 79);
}
#endif
