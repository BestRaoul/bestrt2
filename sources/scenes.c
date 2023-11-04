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

void    balls(void);
void    tweens(void);
void    quads(void);
void	lights0(void);
void	lights(void);
void	lights2(void);
void	cornell(vec3 dimensions);
void	mirror_balls(void);
void	checkers(void);
void	first_animation(void);
void	colored_lights(void);
void	specular(void);
void	earth(void);
void	bumpy(void);
void	mirrors(void);
void	tomato(void);
void	glass_ball(void);
void	normal_reflection(void);
void	pbr_balls(void);

void	test(void);

void    shapes(void);
//.. world, specular, glass, reflection...

void	showoff_1();
void	showoff_2();
void	showoff_3();
void	showoff_4();
void	showoff_5();
void	showoff_6();
void	showoff_7();
void	showoff_8();
void	showoff_9();

void    init_scene(int select)
{
	v.item_count = 0;
	v.light_count = 0;
	v.motion_count = 0;

    switch (select)
    {
        case 1 : balls(); break;
        case 2 : tweens(); break;
        case 3 : quads(); break;
		case 0 : lights0(); break; 
		case 4 : lights(); break; 
		case -4 : lights2(); break; 
		case 5 : mirror_balls(); break;
		case 6 : checkers(); break;
		case 7 : first_animation(); break;
		case 8 : colored_lights(); break;
		case 9 : specular(); break; //specular sucks
		case 10: earth(); break;
		case 11: bumpy(); break;
		case 12: mirrors(); break;
		case 13: tomato(); break;
		case 14: glass_ball(); break;
		case 15: normal_reflection(); break;
		case 16: pbr_balls(); break;
		//point lights
		//normal_bump
		//specular
		//directional_ligth

        case 99 : shapes(); break;

		case 101: showoff_1(); break;
		case 102: showoff_2(); break;
		case 103: showoff_3(); break;
		case 104: showoff_4(); break;
		case 105: showoff_5(); break;
		case 106: showoff_6(); break;
		case 107: showoff_7(); break;
		case 108: showoff_8(); break;
		case 109: showoff_9(); break;

		case 100: test(); break;
		
		default: write(2, "NO scene selected..\n", 21); exit(1); break;
    }
}

void    default_cam(void)
{
	v.w = 16 * (2*2*3*5);
	v.h = 9 * (2*2*3*5);
	v.upscale = 3;
	
	v.camera_pos = v3(0, 0, 3);
	v.camera_rot = v3(0, MYPI, 0);
	v.lookat_toggle = 0;
	v.lookat = v3(0, 0, 0);
	v.vup = v3(0,1,0);
	v.near = 0.2;
	v.far = 1000;
	v.vfov = 50;
	v.render_mode = RAYTRACE_STEPS;
	v.orthographic_toggle = 0;

	v.defocus_angle = 0;
    v.focus_dist = 10;


	v.time_speed = 1;

	//Sampling & depth
	v.max_depth = 6;
	v.max_samples = 10;
	v.samples_per_step = 5;

	//Animation rendering stats
	v.animation_duration = 1.0;
	v.animation_framerate = 24;
	v.animation_speed = 1.0;
	v.animation_render_mode = RAYTRACE;
	v.animation_loops = 0;
	v.rendering_movie = False;

	//Global Textures
	v.use_background = True;
	v.background = cc3(WHITE);
	
	v.use_IBL = False;
	v.irradiance_map = NO_MAP;
	v.blurry_irradiance_map = NO_MAP;

	v.uv_debug = from_bmp("textures/debug/uv_check.bmp");

	v.solo_lighting = 0;
}

//SHOWOFF

void	test(void)
{
	default_cam();
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
    material material_right  = new_metal(c3(0.8, 0.6, 0.2), 0.02);

    
	add_item(v3( 1, 0, 0),		v_3(0.2),	v3(), material_right, SPHERE);
	add_item(v3( 1, 0, 0),		v_3(1),		v3(), material_right, PLANE);
	return;
}

//!1 : green plane, 3 balls (gold, blue, hollow glass)
//ok, 1 noise
void	showoff_1(void)
{
	default_cam();
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

	double _h = 3;
	double _s = 1;

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
	add_item(v3(.5, -.3,-.4),	v3(.4,.7,.4),	v3(), cylinder, CYLINDER);
	
	material box = new_lambertian(WHITE_MAP);
	box.metalic = FULL_MAP;
	add_item(v3(-.5, -.7,.6),	v_3(.3),	v3(0,-MYPI/10), box, BOX);
	
	material lit = new_light(WHITE_MAP, 2.0);
	add_item(v3( 0, .99, 0),	v_3(.8),	v3(MYPI), lit, SS_QUAD);

	add_lamp(WHITE, v3(.9,.9,.9), 1.0, False);
}

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
	
	double *ry = &(v.items[0].transform.rot.y);
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
	v.use_IBL = 1;
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


	double s = 5;
	double d0 = 5;
	double d1 = 3;

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
	v.w = v.h = 500;
	v.camera_pos = v3(0,3,8);
	v.lookat_toggle = 1;
	v.max_samples = 1;
	v.samples_per_step = 2;
	v.background = cc3(vrgb(254, 250, 224));

	double s = 1.5;

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
	v.irradiance_map = from_bmp("textures/hdri/grass4k.bmp");
	v.use_IBL = 1;
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
	double sh = 1.2;
	double sv = 1.2;
	for (int i=0; i<=count; i++)
	{
		double ratio = 1.0 - (double)i/(double)count;
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
}


//Historic -------------------------

void    balls(void)
{
	default_cam();
    v.render_mode = RAYTRACE;
	v.lookat_toggle = 1;
	v.lookat = v3(0,0,0);
	v.camera_pos = v3(2.16, 1.45, 1.57);
	v.vfov = 20;
	v.ambient = 0;

    material material_ground = new_lambertian(checkerboard(.5, c3(.2,.3,.1), c3(.9, .9, .9)));
    material material_center = new_lambertian(c3(0.1, 0.2, 0.4));
    material material_left	 = new_dielectric(c3(1,1,1), 1.5);
    material material_right  = new_metal(c3(0.8, 0.6, 0.2), 0.02);

    add_item(v3( 0, -.5, 0),	v_3(1),	v3(), material_ground, PLANE);
    
    add_item(v3( 0, 0, 0),		v_3(0.5),	v3(), material_center, SPHERE);
	add_item(v3(-1, 0, 0),		v_3(0.5),	v3(), material_left, SPHERE);
	add_item(v3(-1, 0, 0),		v_3(-0.4),	v3(), material_left, SPHERE);
	add_item(v3( 1, 0, 0),		v_3(0.5),	v3(), material_right, SPHERE);
}

void    tweens(void)
{
    default_cam();
	v.render_mode = RASTER;
	v.orthographic_toggle = 1;
	v.camera_pos = v3(0, 0, 12.5);
	v.motion_enabled = 1;

	material mr = new_lambertian(c3(1, 0, 0));
	material mg = new_lambertian(c3(0, 1, 0));
	material mb = new_lambertian(c3(0, 0, 1));
    
	int _x = 7;
	int _y = 1.5;
    add_item(v3( _x--, _y),	v_3(0.5), v3(), mr, SPHERE);
    add_item(v3( _x--, _y),	v_3(0.5), v3(), mg, SPHERE);
    add_item(v3( _x--, _y),	v_3(0.5), v3(), mb, SPHERE);

	_x = -1;
	_y = 2.5;
    add_item(v3( _x-=2, _y),	v_3(0.5), v3(), mr, SPHERE);
    add_item(v3( _x-=2, _y),	v_3(0.5), v3(), mg, SPHERE);
    add_item(v3( _x-=2, _y),	v_3(0.5), v3(), mb, SPHERE);

	_x = 9;
	_y = -2.5;
	add_item(v3( _x-=2, _y),	v_3(0.8), v3(), mr, SPHERE);
    add_item(v3( _x-=2, _y),	v_3(0.8), v3(), mg, SPHERE);
    add_item(v3( _x-=2, _y),	v_3(0.8), v3(), mb, SPHERE);

	_x = -1;
	_y = -2.5;
    add_item(v3( _x-=2, _y),	v_3(0.5), v3(), mr, SPHERE);
    add_item(v3( _x-=2, _y),	v_3(0.5), v3(), mg, SPHERE);
    add_item(v3( _x-=2, _y),	v_3(0.5), v3(), mb, SPHERE);

	add_motion(&(v.items[0].transform.pos.y), 1.5, 3.5, lerpd);
	add_motion(&(v.items[1].transform.pos.y), 1.5, 3.5, ping_pong);
	add_motion(&(v.items[2].transform.pos.y), 1.5, 3.5, ping_pong_2);

	add_motion(&(v.items[3].transform.scale.x), 0, 2, lerpd);
	add_motion(&(v.items[4].transform.scale.y), 0, 2, ping_pong);
	add_motion(&(v.items[5].transform.scale.z), 0, 2, ping_pong_2);

	add_motion(&(v.items[6].transform.rot.x), 0, MYPI, lerpd);
	add_motion(&(v.items[7].transform.rot.y), 0, MYPI, ping_pong);
	add_motion(&(v.items[8].transform.rot.z), 0, MYPI, ping_pong_2);

	add_motion(&(v.items[ 9].mat.base_color.color_value.z), 0, 255, lerpd);
	add_motion(&(v.items[10].mat.base_color.color_value.z), 0, 255, ping_pong);
	add_motion(&(v.items[11].mat.base_color.color_value.z), 0, 255, ping_pong_2);
}

void    quads(void)
{
    default_cam();
    v.render_mode = RAYTRACE;
	v.camera_pos = v3(0, 0, 2.8);
	v.vfov = 80;

    material material_wall_top   = new_lambertian(c3(1.0, 0.5, 0.0));
    material material_wall_back  = new_lambertian(c3(0.2, 1.0, 0.2));
	
	material material_wall_left  = new_lambertian(c3(1.0, 0.2, 0.2));
	material material_wall_bot   = new_lambertian(c3(0.2, 0.8, 0.8));
	material material_wall_right = new_lambertian(c3(0.2, 0.2, 1.0));

	double _w = 1;
	double _h = 1;
	double _d = 1;
	
	double s = 0.4;

	add_item(v3( 0, _h+s, 0),	v3(_w, 1, _d),	v3(MYPI,0,0), 	material_wall_top,	QUAD);
	add_item(v3( 0, 0,-_d-s),	v3(_w, 1, _h),	v3(MYPI/2,0,0), material_wall_back,	QUAD);
	
	add_item(v3( _w+s, 0, 0),	v3(_h, 1, _d),	v3(0,0,MYPI/2),	material_wall_left,	QUAD);
	add_item(v3( 0,-_h-s, 0),	v3(_w, 1, _d),	v3(0,0,0), 		material_wall_bot, 	QUAD);
	add_item(v3(-_w-s, 0, 0),	v3(_h, 1, _d),	v3(0,0,-MYPI/2),material_wall_right,QUAD);
}

void	lights0(void)
{
	default_cam();
    v.render_mode = RAYTRACE;
	v.camera_pos = v3(-10.172291, 2.782435, -0.421150);
	v.camera_rot = v3(15.523259, 97.909557, 0.000000);
	v.camera_rot = v_scal(v.camera_rot, DEG2RAD);
	v.vfov = 30;
	v.background = cc3(BLACK);

    material material_ground = new_lambertian(c3(0.2, 0.3, 1.0));
    material material_center = new_lambertian(c3(1.0,   0, 1.0));
    material material_right  = new_light(c3(0.8, 0.5, 0.1), 20.0);
	material_right.base_color = c3(0.8, 0.5, 0.1);

    add_item(v3( 0, -101, 0),	v_3(100),	v3(), material_ground, SPHERE);
    
    add_item(v3( 0, 0, 0),		v_3(1),	v3(), material_center, SPHERE);
	add_item(v3( 32, 4, -32),		v_3(20),	v3(), material_right, SPHERE);
}

void	lights(void)
{
	default_cam();
	v.camera_pos = v3(11.099445, 1.100155, 4.864002);
	v.camera_rot = v_scal(v3(0.000000, 135.000000, 0.000000), DEG2RAD);
	v.background = from_func(shit_sky_background);

	material	b0 = new_lambertian(c3(.4, .1, .8));
	material	b1 = new_lambertian(c3(.1, .1, .1));
	material	b2 = new_lambertian(c3(.2, .0, .9));
	material	b3 = new_lambertian(c3(.0, .8, .2));
	material	b4 = new_lambertian(c3(.8, .2, .0));
	material	b5 = new_lambertian(c3(.9, .9, .9));

	add_item(v3( 0,	-100, 0),	v_3(100),	v3(), 	b0,	SPHERE);
	add_item(v3( 20, 	-1.2, .5),	v_3(.7),	v3(), 	b1,	SPHERE);
	add_item(v3( 18.5, -1, 0),	v_3(.9),v3(), 	b2,	SPHERE);
	add_item(v3( 17, 	-.8, -1.5),v_3(1.1),v3(), 	b3,	SPHERE);
	add_item(v3( 15, 	-.4, -3),v_3(1.3),v3(), 	b4,	SPHERE);
	add_item(v3( 14, 	0, -7),v_3(2),v3(), 	b5,	SPHERE);

	material	light = new_light(WHITE_MAP, 13.0);
	//add_item(v3( 50, -1, -6),	v_3(15),v3(), 	light,	SPHERE);
	v.upscale = 1;
}

void	lights2(void)
{
	default_cam();
    v.render_mode = RAYTRACE;
	v.camera_pos = v3(-1.518677, 1.338787, 2.834727);
	v.lookat_toggle = 1;
	v.lookat = v3(0, 1, 0);
	v.background = cc3(BLACK);

    material material_ground = new_lambertian(c3(.7,.8,.9));
    add_item(v3(0), v_3(100),	v3(), material_ground, PLANE);

	material light = new_light(WHITE_MAP, 40);
    add_item(v3(0), v_3(.1),	v3(), light, SPHERE);


	double _d = 0;
	double _r = 0;
	double _beta = 0;
	vec3 pp = v3();
	for (int i=0; i<50; i++)
	{
		material m = new_lambertian(c3(1, random_double(), random_double()));
		vec3	p = rotate_y(v3(_d, 0, 0), _beta);
    	double radius = vec_dist(p,pp)/2;
		p.y += radius;
		add_item(p, v_3(radius), v3(), m, SPHERE);
		_d += 0.5;
		_beta += 15*DEG2RAD;
		pp = p;
	}

}

//top, back, front
//left, bot, right
// vec3(16 / 9, 1, 1)
void    cornell(vec3 dimensions)
{
    default_cam();
	v.camera_pos = v3(0, 0, MYPI);
	v.background = cc3(BLACK);
	v.irradiance_map = NO_MAP;
	v.lookat = v3(0, 0, 0);
	v.lookat_toggle = 1;

    material wall = new_lambertian(c3(.73, .73, .73));
	
	material wall_left  = new_lambertian(c3(.12, .45, .15));
	material wall_bot   = new_lambertian(checkerboard(.2, c3(.1, .2, .8), c3(.3, .4, .8)));
	material wall_right = new_lambertian(c3(.65, .05, .05));

	if (v_len(dimensions)==0.0) dimensions = v3(16.0 / 9.0, 1, 1);
	double _w = dimensions.x;
	double _h = dimensions.y;
	double _d = dimensions.z;
	
	double s = 0;

	add_item(v3( 0, _h+s, 0),	v3(_w, 1, _d),	v3(MYPI,0,0), 	wall,		SS_QUAD);
	add_item(v3( 0, 0,-_d-s),	v3(_w, 1, _h),	v3(MYPI/2,0,0), wall,		SS_QUAD);
	add_item(v3( 0, 0,_d+s),	v3(_w, 1, _h),	v3(-MYPI/2,0,0),wall,		SS_QUAD);
	
	add_item(v3( _w+s, 0, 0),	v3(_h, 1, _d),	v3(0,0,MYPI/2),	wall_left,	SS_QUAD);
	add_item(v3( 0,-_h-s, 0),	v3(_w, 1, _d),	v3(0,0,0), 		wall_bot, 	SS_QUAD);
	add_item(v3(-_w-s, 0, 0),	v3(_h, 1, _d),	v3(0,0,-MYPI/2),wall_right,SS_QUAD);
}

void	mirror_balls(void)
{
	cornell(v3(0));

	
	material mirror = new_metal(c3(1,1,1), 0);
	add_item(v3( -1, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE);
	add_item(v3( 1, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE);

	t_item *sph1 = &(v.items[7]);
	t_item *sph2 = &(v.items[8]);
	add_motion(&(sph1->transform.pos.x), -1, -.5, easeInOutCubic);
	add_motion(&(sph2->transform.pos.x), 1, .5, easeInOutCubic);

	add_lamp(WHITE, v3(15.9/9.0, .9, -.9), 5, False);
	add_lamp(WHITE, v3(-15.9/9.0, .9, -.9), 5, False);
	v.max_depth = 1;
	v.max_samples = 1;
}

void	checkers(void)
{
	default_cam();
	v.camera_pos = v3(3.574662, 3.564830, -2.081380);
	v.lookat_toggle = 1;
	v.lookat = v3(0, 2, 0);

	material ground = new_lambertian(checkerboard(.5,
		c3(.1, .1, .1),//checkerboard(1, c3(.1,.1,.1), c3(.9, .9, .9)),
		c3(.9, .9, .9)));
	material sphere = new_lambertian(checkerboard(.05, c3(.8,.9,.1), c3(.8, .1, .9)));

    add_item(v3( 0, 0, 0),	v_3(1),	v3(), ground, PLANE);
    add_item(v3( 0, 2, 0),		v_3(1),	v3(), sphere, SPHERE);
}

void	first_animation(void)
{
	default_cam();
	v.camera_pos = v3(0, 3, 0);
	v.lookat_toggle = 1;
	v.lookat = v3(0, 2, 0);
	v.render_mode = RASTER;
	v.motion_enabled = 1;
	v.time_speed = 0.4;

	material ground = new_metal(checkerboard(.5,
		c3(.2, .3, .1),//checkerboard(1, c3(.1,.1,.1), c3(.9, .9, .9)),
		c3(.9, .9, .9)), 0);
	material sphere = new_lambertian(checkerboard(.05, c3(.9,.6,.1), c3(.3, .1, .9)));
	material sr 	= new_lambertian(c3(.8,.2,.1));

    add_item(v3( 0, 0, 0),	v_3(1),	v3(), ground, PLANE);
    add_item(v3( 0, 2, 0),	v_3(1),	v3(), sphere, SPHERE);
    add_item(v3( 0, 1, 0),	v_3(.1),v3(), sr	, SPHERE);

	t_item *sph = &(v.items[1]);
	t_item *sph2 = &(v.items[2]);
	add_motion(&(sph->transform.pos.y), 1.6, 2.4, ping_pong_2);
	add_motion(&(sph2->transform.pos.x), -2, 2, cos_tween);
	add_motion(&(sph2->transform.pos.z), -2, 2, sin_tween);
	add_motion(&(sph2->transform.pos.y), 2.6, 1.3, ping_pong);

	add_motion(&(v.camera_pos.x), -4, 4, sin_tween);
	add_motion(&(v.camera_pos.z), -4, 4, cos_tween);
}

void    colored_lights(void)
{
    default_cam();
	v.background = cc3(BLACK);
	v.lookat = v3(0, 1, 0);
	v.lookat_toggle = 1;
	v.camera_pos = v3(4.191477, 5.022816, 4.439668);
	
	material ground = new_lambertian(checkerboard(.5,
		c3(.1, .1, .1),
		c3(.9, .9, .9)));
	material sphere = new_lambertian(c3(1,1,1));

    add_item(v3( 0, 0, 0),	v_3(1),	v3(), ground, PLANE);
    add_item(v3( 0, 1, 0),	v_3(1),	v3(), sphere, SPHERE);

	material light_red = new_light(c3(1, 0, 0), 1.0);
	material light_gre = new_light(c3(0, 1, 0), 1.0);
	material light_blu = new_light(c3(0, 0, 1), 1.0);

	double _h = 3;
	double _s = 1;
    //add_item(v3( -_s, _h, -_s),	v_3(.5),v3(), light_red	, SPHERE);
   // add_item(v3(  _s, _h, -_s),	v_3(.5),v3(), light_gre	, SPHERE);
   // add_item(v3( -_s, _h,  _s),	v_3(.5),v3(), light_blu	, SPHERE);

	add_lamp(v3(1, 0, 0), v3(-_s,_h,-_s), 5.0, False);
	add_lamp(v3(0, 1, 0), v3( _s,_h,-_s), 5.0, False);
	add_lamp(v3(0, 0, 1), v3(-_s,_h, _s), 5.0, False);

	add_motion(&(v.camera_pos.x), -4, 4, sin_tween);
	add_motion(&(v.camera_pos.z), -4, 4, cos_tween);

	v.upscale = 3;
	v.max_depth = 50;
	v.motion_enabled = 0;
	v.render_mode = RASTER;
	v.time_speed = 0.2;

	v.animation_duration = 5;
	v.animation_speed = 0.2;
	v.animation_framerate = 24;
	v.animation_loops = 3;
	v.animation_render_mode = RAYTRACE;
	v.max_samples = 10;
	v.max_depth = 2;
}

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
    v.render_mode = RAYTRACE_STEPS;
	v.camera_pos = v3(-1.211402, 0.165707, 2.180947);
	v.background = cc3(BLACK);
	v.lookat_toggle = 1;
	v.lookat = v3(0, 0, 0);
	v.upscale = 2;
	v.max_depth = 50;
	v.w = 20 * 16.0; //240 for 4k
	v.h = 20 * 9.0;
	v.upscale = 1;
	v.ambient = 0;
	v.irradiance_map = NO_MAP;
	v.max_samples = 1;
	v.samples_per_step = 1;
	double fuzzzz = 0.0;

    material material_wall = new_metal(c3(.73, .73, .73), fuzzzz);
	
	material material_wall_left  = new_metal(c3(.12, .45, .15), fuzzzz);
	material material_wall_bot   = new_metal(checkerboard(.1, c3(.1, .2, .8), c3(.3, .4, .8)), fuzzzz);
	material material_wall_right = new_metal(c3(.65, .05, .05), fuzzzz);

	double _w = 16.0/9.0;
	double _h = 1;
	double _d = 1;
	
	double s = 0;

	add_item(v3( 0, _h+s, 0),	v3(_w, 1, _d),	v3(MYPI,0,0), 	material_wall,		SS_QUAD);
	add_item(v3( 0, 0,-_d-s),	v3(_w, 1, _h),	v3(MYPI/2,0,0), material_wall,		SS_QUAD);
	add_item(v3( 0, 0,_d+s),	v3(_w, 1, _h),	v3(-MYPI/2,0,0),material_wall,		SS_QUAD);
	
	add_item(v3( _w+s, 0, 0),	v3(_h, 1, _d),	v3(0,0,MYPI/2),	material_wall_left,	SS_QUAD);
	add_item(v3( 0,-_h-s, 0),	v3(_w, 1, _d),	v3(0,0,0), 		material_wall_bot, 	SS_QUAD);
	add_item(v3(-_w-s, 0, 0),	v3(_h, 1, _d),	v3(0,0,-MYPI/2),material_wall_right,SS_QUAD);

	material light = new_light(WHITE_MAP, 3.0);//checkerboard(0.2, c3(.1,.1,.1), c3(3,3,3)));
	light.metalic = BW_MAP(1.0);
	add_item(v3( 0, _h+s-1e-8, 0),	v_3(.8),	v3(MYPI,0,0), 	light,	SS_QUAD);

	material mirror = new_metal(c3(1,1,1), fuzzzz);
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

//UV test

void	bumpy(void)
{
	default_cam();
    v.render_mode = RAYTRACE_STEPS;
	v.camera_pos = v3(0.0, 1.5, 2.25);
	v.lookat_toggle = 1;
	v.max_depth = 2;
	v.upscale = 1;
	v.w = 300;
	v.h = 300;
	v.background = cc3(BLACK);
	v.max_samples = 1;

	// material light = new_light(c3(0,1,1), 2.0);
	// add_item(v3( 0, 1, -2),	v_3(.5),	v3(MYPI/2,0,0), 	light,	SS_QUAD);

	texture ea = c3(1,1,1);
	texture ea_bump = from_bmp("textures/shapes.bmp");
	material earth_bumpy = new_lambertian_bump(ea,ea_bump);
	earth_bumpy.specular = BW_MAP(1.0);
	earth_bumpy.roughness = BW_MAP(.0);
	earth_bumpy.metalic = BW_MAP(.0);

	add_item(v3(),	v_3(1),	v3(), 	earth_bumpy,	QUAD);

	v.animation_duration = 5;
	v.animation_speed = 0.2;
	v.animation_framerate = 24;
	v.time_speed = v.animation_speed;

	v.max_samples = 1;
	v.samples_per_step = 1;
	v.max_depth = 2;
	v.animation_render_mode = RAYTRACE_STEPS_3;
	v.irradiance_map = from_bmp("textures/emptyroom1k.bmp");

	add_lamp(v3(1,.2,.4), v3(-1, .5, .3), 20.0, False);
	vec3	*lp = &v.lights[0].transform.pos;
	double d = 1;
	//add_motion(&(lp->x), -d, d, sin_tween);
	//add_motion(&(lp->z), -d, d, cos_tween);
	double	*rot_y = &v.items[1].transform.rot.y;
	add_motion(rot_y, 0, -MYPI*2, lerpd);
	//add_motion(str, -5, 5, ping_pong);
}

void	normal_reflection(void)
{
	default_cam();
	v.camera_pos = v3(-2.611575, 1.628231, 3.254508);
	v.lookat_toggle = 1;
	v.lookat = v3(0, .4, 0);
	v.w = 600;
	v.h = 400;
	v.upscale = 1;
	v.background = from_func(shit_sky_background);

	material ground = new_lambertian(checkerboard(.5,
		c3(.9, .5, .1),//checkerboard(1, c3(.1,.1,.1), c3(.9, .9, .9)),
		c3(.6, .6, .6)));
	ground.roughness = BW_MAP(.02);
	texture shaaapes = from_bmp("textures/shapes.bmp");
	ground.normal = checkerboard(.5,
		shaaapes,//checkerboard(1, c3(.1,.1,.1), c3(.9, .9, .9)),
		shaaapes);
	ground.normal_strength = v_3(-.5);
	ground.metalic = BW_MAP(.8);
	ground.specular = BW_MAP(0.0);
	material sphere = new_lambertian(from_bmp("textures/earthmap1k.bmp"));
	sphere.normal = from_bmp("textures/earthnormal1k.bmp");
	sphere.specular = BW_MAP(0.0);
	sphere.roughness = BW_MAP(1.0);

    add_item(v3( 0, 0, 0),	v_3(1),	v3(), ground, PLANE);
    add_item(v3( 0, 1.2, 0),		v_3(1),	v3(), sphere, SPHERE);
	(void) sphere;
	//add_motion(&(v.camera_pos.x), 4.5, -4.5, sin_tween);
	//add_motion(&(v.camera_pos.z), 4.5, -4.5, cos_tween);
	//add_motion(&(v.sunDirection))

	//Animation
	v.animation_duration = 2;
	v.animation_framerate = 24;
	v.animation_speed = .5;
	v.time_speed = 0.5;

	add_lamp(v3(1,.8,.4), v3(-10, 1.2, 0), 200.0, False);
	v.max_depth = 3;
	v.max_samples = 5;

	vec3	*lp = &v.lights[0].transform.pos;
	double d = 10;
	add_motion(&(lp->x), -d, d, sin_tween);
	add_motion(&(lp->z), -d, d, cos_tween);
}

void	pbr_balls(void)
{
	default_cam();
	v.background = cc3(BLACK);
	v.camera_pos = v3(0, 0, 600);
	v.vfov = 1;
	v.lookat_toggle = 1;
	v.max_samples = 1;
	v.upscale = 1;
	v.max_depth = 0;


	material m = new_lambertian(c3(.7, .7, .7));

	int count = 10;
	int _y = 4;
	
	m.base_color = c3(0.8, 0.6, 0.2); //yellow
	m.specular = NO_MAP;
	m.roughness = BW_MAP(0.2);
	for (double i=0; i<=count; i++)
	{
    	m.metalic = BW_MAP(0.1 * i);
		add_item(v3((count/2 - i)*1.5, _y, 0),	v_3(.5),v3(), m, SPHERE);
	}
	_y -= 1.5;

	m.base_color = c3(1,0,0); //red
	m.metalic = NO_MAP;
	m.roughness = BW_MAP(0.);
	for (double i=0; i<=count; i++)
	{
    	m.specular = BW_MAP(0.1 * i);
		add_item(v3((count/2 - i)*1.5, _y, 0),	v_3(.5),v3(), m, SPHERE);
	}
	_y -= 1.5;

	m.base_color = c3(.6,.6,.8); //blueish
	m.metalic  = NO_MAP;
	m.specular = NO_MAP;
	for (double i=0; i<=count; i++)
	{
    	m.roughness = BW_MAP(0.1 * i);
		add_item(v3((count/2 - i)*1.5, _y, 0),	v_3(.5),v3(), m, SPHERE);
	}
	_y -= 2.5;

	m.base_color = c3(139.0/256.0,173.0/256.0,158.0/256.0); //greenish
	m.metalic  = NO_MAP;
	m.specular = NO_MAP;
	m.roughness= NO_MAP;
	for (double i=0; i<=count; i++)
	{
		m.transmission = BW_MAP(0.1 * i);
		add_item(v3((count/2 - i)*1.5, _y, 0),	v_3(.5),v3(), m, SPHERE);
	}
	_y -= 2.5;

	m.transmission = FULL_MAP;
	for (double i=0; i<=count; i++)
	{
		m.transmission_roughness = BW_MAP(0.1 * i);
		add_item(v3((count/2 - i)*1.5, _y, 0),	v_3(.5),v3(), m, SPHERE);
	}

	material m2 = new_lambertian(c3(1,1,1));
	//m2.metalic = BW_MAP(1.0);
	m2.roughness = BW_MAP(0*1.0);
	m2.specular = BW_MAP(1.0);
	m2.ior = 10;
	add_item(v3(),	v_3(.5),v3(), m2, SPHERE);
	material check = new_lambertian(checkerboard(.05, c3(.3,.3,.3), c3(.5,.5,.5)));
//	add_item(v3(0,-.5),v_3(10),v3(), check, QUAD);
	material l = new_lambertian(c3(1,1,1));
	material r = new_lambertian(c3(.5,.6,1));
//	add_item(v3(-1.3),	v_3(.5),v3(), l, SPHERE);
//	add_item(v3(1.3),	v_3(.5),v3(), r, SPHERE);

	v.vfov = 0.1;
	v.w = 300;
	v.h = 300;
	// v.camera_pos = v3(0.252365, 0.65293, 2.384792);
	// v.vfov = 50;
	v.max_depth = 2;
	v.max_samples = 1;
	v.render_mode = RAYTRACE_STEPS;
	v.irradiance_map = from_bmp("textures/emptyroom1k.bmp");//BW_MAP(0.5);

	add_lamp(WHITE, v3(-1,-1,-1), 2.0, True);
}

//__NOT__
void    shapes(void)
{
    default_cam();
	v.camera_pos = v3(-1.743579, 1.213285, 9.192838);

    
	add_item(	v3(3, 3), v_3(1), 	v3(), new_lambertian(c3(.2, 0, 1)),		SPHERE);
	add_item(	v3(3, 0), v_3(1), 	v3(), new_lambertian(c3(.2, 1, .4)),	CYLINDER);
	add_item(	v3(-3, 3), v_3(1),	v3(), new_lambertian(c3(0, .6, .9)),	CONE);
    add_item(	v3(-3, 0), v_3(1),	v3(), new_lambertian(c3(1, .35, .1)),	BOX);
	add_item(	v3(-6, 0),v3(1.5,1,1), v3(), new_lambertian(c3(.6, .8, .2)),BOX);
}
