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

void    init_scene(void)
{
	v.item_count = 0;
	v.items = NULL;

	v.motion_count = 0;
	v.motions = NULL;

    switch (109)
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
		case 106: showoff_2(); break;
		case 107: showoff_2(); break;

		case 109: test(); break;
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
	v.near_plane = 0.2;
	v.far_plane = 1000;
	v.vfov = 50;

	v.orthographic_toggle = 0;
	v.render_mode = RAYTRACE_STEPS;

	v.background_color = sky_background;
	v.time_speed = 1;

	v.max_depth = 6;

	v.uv_debug = from_bmp("uv_check.bmp");

	v.max_samples = 10;
	v.samples_per_step = 5;

	v.animation_duration = 1.0;
	v.animation_framerate = 24;
	v.animation_speed = 1.0;
	v.animation_render_mode = RAYTRACE;
	v.animation_loops = 0;
	v.rendering_movie = False;

	v.skyColorZenith = v3(0.3, 0.3, 0.9);
	v.skyColorHorizon = WHITE;
	v.groundColor = v3(0.3, 0.3, 0.3);

	v.sunFocus = 10;
	v.sunIntensity = 1.0;
	v.sunDirection = v_norm(v3(1, 1, 1));

	v.irradiance_map = BW_MAP(.03);
}

//SHOWOFF

void	test(void)
{
	default_cam();
    v.render_mode = RAYTRACE;
	v.lookat_toggle = 0;
	v.lookat = v3(0,0,0);
	v.camera_pos = v3(0,0,4.5);
	v.vfov = 50;
	v.ambient = 0;
	v.background_color = shit_sky_background;
	v.irradiance_map = NO_MAP;
	v.max_samples = 1;
	v.max_depth = 4;
	v.w =  500; v.h = 500;
	v.upscale = 1;

    material material_ground = new_lambertian(checkerboard(.5, c3(.2,.3,.1), c3(.9, .9, .9)));
    material material_center = new_lambertian(c3(0.1, 0.2, 0.4));
    material material_left	 = new_dielectric(c3(1,1,1), 1.5);
	material_left.transmission = 1.0;
    material material_right  = new_metal(c3(0.8, 0.6, 0.2), 0.02);

    add_item((t_item){v3( 0, -.5, 0),	v_3(1),	v3(), material_ground, PLANE});
    
    add_item((t_item){v3( 0, 0, 0),		v3(1,.5,.5),	v3(0), material_center, SPHERE});
	add_item((t_item){v3(-1.5, 0, 0),	v3(.5,.5,1),	v3(), material_left, SPHERE});
	add_item((t_item){v3( 1.5, 0, 0),	v3(.5,1,.5),	v3(), material_right, SPHERE});
}

//1 : green plane, 3 balls (gold, blue, hollow glass)
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
	v.background_color = white_background;
	v.irradiance_map = NO_MAP;
	v.max_samples = 10;
	v.max_depth = 3;
	v.upscale = 1;

    material material_ground = new_lambertian(checkerboard(.5, c3(.2,.3,.1), c3(.9, .9, .9)));
    material material_center = new_lambertian(c3(0.1, 0.2, 0.4));
    material material_left	 = new_dielectric(c3(1,1,1), 1.5);
	material_left.transmission = 1.0;
    material material_right  = new_metal(c3(0.8, 0.6, 0.2), 0.02);

    add_item((t_item){v3( 0, -.5, 0),	v_3(1),	v3(), material_ground, PLANE});
    
    add_item((t_item){v3( 0, 0, 0),		v_3(0.5),	v3(), material_center, SPHERE});
	add_item((t_item){v3(-1, 0, 0),		v_3(0.5),	v3(), material_left, SPHERE});
	//add_item((t_item){v3(-1, 0, 0),		v_3(-0.4),	v3(), material_left, SPHERE});
	add_item((t_item){v3( 1, 0, 0),		v_3(0.5),	v3(), material_right, SPHERE});
}

//2 : ball 3 point lights, checkplane
//good, 1 noise
void	showoff_2(void)
{
	default_cam();
	v.background_color = black_background;
	v.irradiance_map = NO_MAP;
	v.lookat = v3(0, 1, 0);
	v.lookat_toggle = 1;
	v.camera_pos = v3(0, 1.0, 3.5);
	v.upscale = 3;
	v.samples_per_step = 3;
	
	material check = new_lambertian(checkerboard(.5,
		c3(.1, .1, .1),
		c3(.9, .9, .9)));
	material sphere = new_lambertian(c3(1,1,1));

    add_item((t_item){v3( 0, 0, 0),	v_3(1),	v3(), check, PLANE});
    add_item((t_item){v3( 0, 1, 0),	v_3(1),	v3(), sphere, SPHERE});

	double _h = 3;
	double _s = 1;

	v.light_count = 3;
	v.lights = malloc(sizeof(t_light)*v.light_count);
	v.lights[0] = (t_light){v3(1, 0, 0),
						v3(-_s,_h, _s),
						v3(),
						5.0,
						False};
	v.lights[1] = (t_light){v3(0, 1, 0),
						v3( _s,_h, _s),
						v3(),
						5.0,
						False};
	v.lights[2] = (t_light){v3(0, 0, 1),
						v3(-_s,_h,-_s),
						v3(),
						5.0,
						False};
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
	add_item((t_item){v3( 0, 0, 0),	v_3(.5),	v3(), ball, SPHERE});

	material lit = new_lambertian(NO_MAP);
	lit.emission = WHITE_MAP;
	lit.emission_strength = 2.0;
	add_item((t_item){v3( 0, .99, 0),	v_3(.8),	v3(MYPI), lit, SS_QUAD});
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

	add_item((t_item){v3( 0, -.3, 0),	v_3(.7),	v3(), 	tom,	SPHERE});

	material light = new_light(WHITE_MAP, 15.0);

	add_item((t_item){v3( 15.9/9.0, .7, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD});
	add_item((t_item){v3( 15.9/9.0,  0, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD});
	add_item((t_item){v3( 15.9/9.0,-.7, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD});
	add_item((t_item){v3(-15.9/9.0,  0, 0),	v_3(.4),	v3(0, 0, -MYPI/2), 	light,	SS_QUAD});
}

//5 : cornell, glass cylinder, mirror box
void	showoff_5(void)
{
	cornell(v3(1,1,1));
	v.w = 900; v.h = 900;
	v.upscale = 3;
	v.max_depth = 0;
	v.samples_per_step = 1;
	v.max_samples = 1;
	v.render_mode = RAYTRACE;

	material cylinder = new_lambertian(c3(1,1,0));
	cylinder.transmission = 0.8;
	cylinder.ior = 1.45;
	add_item((t_item){v3(.5, -.3,-.4),	v3(.4,.7,.4),	v3(), cylinder, CYLINDER});
	
	material box = new_lambertian(WHITE_MAP);
	box.metalic = NO_MAP;
	add_item((t_item){v3(-.4, -.6,.6),		v_3(.3),	v3(0), box, BOX});
	
	// material lit = new_lambertian(NO_MAP);
	// lit.emission = WHITE_MAP;
	// lit.emission_strength = 2.0;
	// add_item((t_item){v3( 0, .99, 0),	v_3(.8),	v3(MYPI), lit, SS_QUAD});

	v.light_count = 2;
	v.lights = malloc(sizeof(t_light)*v.light_count);
	v.lights[0] = (t_light){WHITE,
						v3(.99,.99,.99),
						v3(),
						15.0,
						False};
	v.lights[1] = (t_light){WHITE,
						v3(-.99,-.99,-.99),
						v3(),
						15.0,
						False};
}

//6 : earth in space normal mapped

//7 : box with rough surface, sphere with rust, cylinder watery, ...

//8 : ??? 


//Historic

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

    add_item((t_item){v3( 0, -.5, 0),	v_3(1),	v3(), material_ground, PLANE});
    
    add_item((t_item){v3( 0, 0, 0),		v_3(0.5),	v3(), material_center, SPHERE});
	add_item((t_item){v3(-1, 0, 0),		v_3(0.5),	v3(), material_left, SPHERE});
	add_item((t_item){v3(-1, 0, 0),		v_3(-0.4),	v3(), material_left, SPHERE});
	add_item((t_item){v3( 1, 0, 0),		v_3(0.5),	v3(), material_right, SPHERE});
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
    add_item((t_item){v3( _x--, _y),	v_3(0.5), v3(), mr, SPHERE});
    add_item((t_item){v3( _x--, _y),	v_3(0.5), v3(), mg, SPHERE});
    add_item((t_item){v3( _x--, _y),	v_3(0.5), v3(), mb, SPHERE});

	_x = -1;
	_y = 2.5;
    add_item((t_item){v3( _x-=2, _y),	v_3(0.5), v3(), mr, SPHERE});
    add_item((t_item){v3( _x-=2, _y),	v_3(0.5), v3(), mg, SPHERE});
    add_item((t_item){v3( _x-=2, _y),	v_3(0.5), v3(), mb, SPHERE});

	_x = 9;
	_y = -2.5;
	add_item((t_item){v3( _x-=2, _y),	v_3(0.8), v3(), mr, SPHERE});
    add_item((t_item){v3( _x-=2, _y),	v_3(0.8), v3(), mg, SPHERE});
    add_item((t_item){v3( _x-=2, _y),	v_3(0.8), v3(), mb, SPHERE});

	_x = -1;
	_y = -2.5;
    add_item((t_item){v3( _x-=2, _y),	v_3(0.5), v3(), mr, SPHERE});
    add_item((t_item){v3( _x-=2, _y),	v_3(0.5), v3(), mg, SPHERE});
    add_item((t_item){v3( _x-=2, _y),	v_3(0.5), v3(), mb, SPHERE});

	add_motion(&(v.items[0].pos.y), 1.5, 3.5, lerpd);
	add_motion(&(v.items[1].pos.y), 1.5, 3.5, ping_pong);
	add_motion(&(v.items[2].pos.y), 1.5, 3.5, ping_pong_2);

	add_motion(&(v.items[3].scale.x), 0, 2, lerpd);
	add_motion(&(v.items[4].scale.y), 0, 2, ping_pong);
	add_motion(&(v.items[5].scale.z), 0, 2, ping_pong_2);

	add_motion(&(v.items[6].rot.x), 0, MYPI, lerpd);
	add_motion(&(v.items[7].rot.y), 0, MYPI, ping_pong);
	add_motion(&(v.items[8].rot.z), 0, MYPI, ping_pong_2);

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

    material material_wall_top   = new_lambertian(c3(10.0, 0.5, 0.0));
    material material_wall_back  = new_lambertian(c3(0.2, 1.0, 0.2));
	
	material material_wall_left  = new_lambertian(c3(1.0, 0.2, 0.2));
	material material_wall_bot   = new_lambertian(c3(0.2, 0.8, 0.8));
	material material_wall_right = new_lambertian(c3(0.2, 0.2, 1.0));

	double _w = 1;
	double _h = 1;
	double _d = 1;
	
	double s = 0.4;

	add_item((t_item){v3( 0, _h+s, 0),	v3(_w, 1, _d),	v3(MYPI,0,0), 	material_wall_top,	SS_QUAD});
	add_item((t_item){v3( 0, 0,-_d-s),	v3(_w, 1, _h),	v3(MYPI/2,0,0), material_wall_back,	SS_QUAD});
	
	add_item((t_item){v3( _w+s, 0, 0),	v3(_h, 1, _d),	v3(0,0,MYPI/2),	material_wall_left,	SS_QUAD});
	add_item((t_item){v3( 0,-_h-s, 0),	v3(_w, 1, _d),	v3(0,0,0), 		material_wall_bot, 	SS_QUAD});
	add_item((t_item){v3(-_w-s, 0, 0),	v3(_h, 1, _d),	v3(0,0,-MYPI/2),material_wall_right,SS_QUAD});
}

void	lights0(void)
{
	default_cam();
    v.render_mode = RAYTRACE;
	v.camera_pos = v3(-10.172291, 2.782435, -0.421150);
	v.camera_rot = v3(15.523259, 97.909557, 0.000000);
	v.camera_rot = v_scal(v.camera_rot, DEG2RAD);
	v.vfov = 30;
	v.background_color = black_background;

    material material_ground = new_lambertian(c3(0.2, 0.3, 1.0));
    material material_center = new_lambertian(c3(1.0,   0, 1.0));
    material material_right  = new_light(c3(0.8, 0.5, 0.1), 20.0);
	material_right.base_color = c3(0.8, 0.5, 0.1);

    add_item((t_item){v3( 0, -101, 0),	v_3(100),	v3(), material_ground, SPHERE});
    
    add_item((t_item){v3( 0, 0, 0),		v_3(1),	v3(), material_center, SPHERE});
	add_item((t_item){v3( 32, 4, -32),		v_3(20),	v3(), material_right, SPHERE});
}

void	lights(void)
{
	default_cam();
	v.camera_pos = v3(11.099445, 1.100155, 4.864002);
	v.camera_rot = v_scal(v3(0.000000, 135.000000, 0.000000), DEG2RAD);
	v.background_color = shit_sky_background;

	material	b0 = new_lambertian(c3(.4, .1, .8));
	material	b1 = new_lambertian(c3(.1, .1, .1));
	material	b2 = new_lambertian(c3(.2, .0, .9));
	material	b3 = new_lambertian(c3(.0, .8, .2));
	material	b4 = new_lambertian(c3(.8, .2, .0));
	material	b5 = new_lambertian(c3(.9, .9, .9));

	add_item((t_item){v3( 0,	-100, 0),	v_3(100),	v3(), 	b0,	SPHERE});
	add_item((t_item){v3( 20, 	-1.2, .5),	v_3(.7),	v3(), 	b1,	SPHERE});
	add_item((t_item){v3( 18.5, -1, 0),	v_3(.9),v3(), 	b2,	SPHERE});
	add_item((t_item){v3( 17, 	-.8, -1.5),v_3(1.1),v3(), 	b3,	SPHERE});
	add_item((t_item){v3( 15, 	-.4, -3),v_3(1.3),v3(), 	b4,	SPHERE});
	add_item((t_item){v3( 14, 	0, -7),v_3(2),v3(), 	b5,	SPHERE});

	material	light = new_light(WHITE_MAP, 13.0);
	//add_item((t_item){v3( 50, -1, -6),	v_3(15),v3(), 	light,	SPHERE});
	v.upscale = 1;
}

void	lights2(void)
{
	default_cam();
    v.render_mode = RAYTRACE;
	v.camera_pos = v3(-1.518677, 1.338787, 2.834727);
	v.lookat_toggle = 1;
	v.lookat = v3(0, 1, 0);
	v.background_color = black_background;

    material material_ground = new_lambertian(c3(.7,.8,.9));
    add_item((t_item){v3(0), v_3(100),	v3(), material_ground, PLANE});

	material light = new_light(WHITE_MAP, 40);
    add_item((t_item){v3(0), v_3(.1),	v3(), light, SPHERE});


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
		add_item((t_item){p, v_3(radius), v3(), m, SPHERE});
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
	v.background_color = black_background;
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

	add_item((t_item){v3( 0, _h+s, 0),	v3(_w, 1, _d),	v3(MYPI,0,0), 	wall,		SS_QUAD});
	add_item((t_item){v3( 0, 0,-_d-s),	v3(_w, 1, _h),	v3(MYPI/2,0,0), wall,		SS_QUAD});
	add_item((t_item){v3( 0, 0,_d+s),	v3(_w, 1, _h),	v3(-MYPI/2,0,0),wall,		SS_QUAD});
	
	add_item((t_item){v3( _w+s, 0, 0),	v3(_h, 1, _d),	v3(0,0,MYPI/2),	wall_left,	SS_QUAD});
	add_item((t_item){v3( 0,-_h-s, 0),	v3(_w, 1, _d),	v3(0,0,0), 		wall_bot, 	SS_QUAD});
	add_item((t_item){v3(-_w-s, 0, 0),	v3(_h, 1, _d),	v3(0,0,-MYPI/2),wall_right,SS_QUAD});
}

void	mirror_balls(void)
{
	cornell(v3(0));

	
	material mirror = new_metal(c3(1,1,1), 0);
	add_item((t_item){v3( -1, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE});
	add_item((t_item){v3( 1, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE});

	t_item *sph1 = &(v.items[7]);
	t_item *sph2 = &(v.items[8]);
	add_motion(&(sph1->pos.x), -1, -.5, easeInOutCubic);
	add_motion(&(sph2->pos.x), 1, .5, easeInOutCubic);

	v.lights = malloc(2*sizeof(t_light));
	v.light_count = 2;
	v.lights[0] = (t_light){WHITE,
							v3(15.9/9.0, .9, -.9),
							v3(),
							5, 
							False};
	v.lights[1] = (t_light){WHITE,
							v3(-15.9/9.0, .9, -.9),
							v3(),
							5, 
							False};
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

    add_item((t_item){v3( 0, 0, 0),	v_3(1),	v3(), ground, PLANE});
    add_item((t_item){v3( 0, 2, 0),		v_3(1),	v3(), sphere, SPHERE});
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

    add_item((t_item){v3( 0, 0, 0),	v_3(1),	v3(), ground, PLANE});
    add_item((t_item){v3( 0, 2, 0),	v_3(1),	v3(), sphere, SPHERE});
    add_item((t_item){v3( 0, 1, 0),	v_3(.1),v3(), sr	, SPHERE});

	t_item *sph = &(v.items[1]);
	t_item *sph2 = &(v.items[2]);
	add_motion(&(sph->pos.y), 1.6, 2.4, ping_pong_2);
	add_motion(&(sph2->pos.x), -2, 2, cos_tween);
	add_motion(&(sph2->pos.z), -2, 2, sin_tween);
	add_motion(&(sph2->pos.y), 2.6, 1.3, ping_pong);

	add_motion(&(v.camera_pos.x), -4, 4, sin_tween);
	add_motion(&(v.camera_pos.z), -4, 4, cos_tween);
}

void    colored_lights(void)
{
    default_cam();
	v.background_color = black_background;
	v.lookat = v3(0, 1, 0);
	v.lookat_toggle = 1;
	v.camera_pos = v3(4.191477, 5.022816, 4.439668);
	
	material ground = new_lambertian(checkerboard(.5,
		c3(.1, .1, .1),
		c3(.9, .9, .9)));
	material sphere = new_lambertian(c3(1,1,1));

    add_item((t_item){v3( 0, 0, 0),	v_3(1),	v3(), ground, PLANE});
    add_item((t_item){v3( 0, 1, 0),	v_3(1),	v3(), sphere, SPHERE});

	material light_red = new_light(c3(1, 0, 0), 1.0);
	material light_gre = new_light(c3(0, 1, 0), 1.0);
	material light_blu = new_light(c3(0, 0, 1), 1.0);

	double _h = 3;
	double _s = 1;
    //add_item((t_item){v3( -_s, _h, -_s),	v_3(.5),v3(), light_red	, SPHERE});
   // add_item((t_item){v3(  _s, _h, -_s),	v_3(.5),v3(), light_gre	, SPHERE});
   // add_item((t_item){v3( -_s, _h,  _s),	v_3(.5),v3(), light_blu	, SPHERE});

	v.light_count = 3;
	v.lights = malloc(sizeof(t_light)*v.light_count);
	v.lights[0] = (t_light){v3(1, 0, 0),
						v3(-_s,_h,-_s),
						v3(),
						5.0,
						False};
	v.lights[1] = (t_light){v3(0, 1, 0),
						v3( _s,_h,-_s),
						v3(),
						5.0,
						False};
	v.lights[2] = (t_light){v3(0, 0, 1),
						v3(-_s,_h, _s),
						v3(),
						5.0,
						False};

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

	add_item((t_item){v3( 1.35, 0, 0),	v_3(.4),	v3(), 	sph_1,	SPHERE});
	add_item((t_item){v3(  .45, 0, 0),	v_3(.4),	v3(), 	sph_04,	SPHERE});
	add_item((t_item){v3( -.45, 0, 0),	v_3(.4),	v3(), 	sph_015,	SPHERE});
	add_item((t_item){v3(-1.35, 0, 0),	v_3(.4),	v3(), 	sph_002,	SPHERE});
}

void	mirrors(void)
{
	default_cam();
    v.render_mode = RAYTRACE_STEPS;
	v.camera_pos = v3(-1.211402, 0.165707, 2.180947);
	v.background_color = black_background;
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

	add_item((t_item){v3( 0, _h+s, 0),	v3(_w, 1, _d),	v3(MYPI,0,0), 	material_wall,		SS_QUAD});
	add_item((t_item){v3( 0, 0,-_d-s),	v3(_w, 1, _h),	v3(MYPI/2,0,0), material_wall,		SS_QUAD});
	add_item((t_item){v3( 0, 0,_d+s),	v3(_w, 1, _h),	v3(-MYPI/2,0,0),material_wall,		SS_QUAD});
	
	add_item((t_item){v3( _w+s, 0, 0),	v3(_h, 1, _d),	v3(0,0,MYPI/2),	material_wall_left,	SS_QUAD});
	add_item((t_item){v3( 0,-_h-s, 0),	v3(_w, 1, _d),	v3(0,0,0), 		material_wall_bot, 	SS_QUAD});
	add_item((t_item){v3(-_w-s, 0, 0),	v3(_h, 1, _d),	v3(0,0,-MYPI/2),material_wall_right,SS_QUAD});

	material light = new_light(WHITE_MAP, 3.0);//checkerboard(0.2, c3(.1,.1,.1), c3(3,3,3)));
	light.metalic = BW_MAP(1.0);
	add_item((t_item){v3( 0, _h+s-1e-8, 0),	v_3(.8),	v3(MYPI,0,0), 	light,	SS_QUAD});

	material mirror = new_metal(c3(1,1,1), fuzzzz);
	add_item((t_item){v3( 0, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE});
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

	add_item((t_item){v3( 0, -.3, 0),	v_3(.7),	v3(), 	tom,	SPHERE});

	material light = new_light(WHITE_MAP, 15.0);

	add_item((t_item){v3( 15.9/9.0, .7, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD});
	add_item((t_item){v3( 15.9/9.0,  0, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD});
	add_item((t_item){v3( 15.9/9.0,-.7, 0),	v_3(.2),	v3(0, 0, MYPI/2), 	light,	SS_QUAD});
	add_item((t_item){v3(-15.9/9.0,  0, 0),	v_3(.4),	v3(0, 0, -MYPI/2), 	light,	SS_QUAD});

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
	add_item((t_item){v3( 0, -.3, 0),	v_3(.7),	v3(), 	glass,	SPHERE});

	material ground   = new_lambertian(checkerboard(.2, c3(.1, .2, .8), c3(.3, .4, .8)));
	add_item((t_item){v3( 0,-1, 0),	v3(1, 1, 1),	v3(0,0,0), 		ground, 	PLANE});

	material light = new_light(WHITE_MAP, 5.0);
	add_item((t_item){v3( 0, 1, 0),	v_3(.2),	v3(MYPI, 0), 	light,	SS_QUAD});
}

void	earth(void)
{
	default_cam();
	v.camera_pos = v3(0, 0, 2.5);
	v.w = 500;
	v.h = 500;
	v.upscale = 1;
	v.lookat_toggle = 1;
	v.background_color = white_background;
	v.irradiance_map = WHITE_MAP;//from_bmp("emptyroom1k.bmp");
	v.max_depth = 0;

	texture earth_texture = from_bmp("earthmap1k.bmp");
	material earth = new_lambertian(earth_texture);
	earth.specular = NO_MAP;
	earth.roughness = NO_MAP;
	earth.normal = from_bmp("earthnormal1k.bmp");
	earth.normal_strength = 1;

	material ground = new_lambertian(checkerboard(.25, c3(.1,.1,.1), c3(.9,.9,.9)));
	ground.specular = NO_MAP;
	ground.roughness = NO_MAP;

	add_item((t_item){v3( 0, 0, 0),	v_3(1),	v3(0,MYPI/4), 	earth,	SPHERE});
	add_item((t_item){v3( 0, -1, 0),v3(1,1,1),	v3(), 	ground,	PLANE});

	v.lights = malloc(sizeof(t_light));
	v.light_count = 0;
	v.lights[0] = (t_light){WHITE,
						v3(-1,0,1),
						v3(-1,-1,-1),
						2.0,
						True};
	
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
	v.background_color = black_background;
	v.max_samples = 1;

	// material light = new_light(c3(0,1,1), 2.0);
	// add_item((t_item){v3( 0, 1, -2),	v_3(.5),	v3(MYPI/2,0,0), 	light,	SS_QUAD});

	texture ea = c3(1,1,1);
	texture ea_bump = from_bmp("shapes.bmp");
	material earth_bumpy = new_lambertian_bump(ea,ea_bump);
	earth_bumpy.specular = BW_MAP(1.0);
	earth_bumpy.roughness = BW_MAP(.0);
	earth_bumpy.metalic = BW_MAP(.0);

	add_item((t_item){v3(),	v_3(1),	v3(), 	earth_bumpy,	QUAD});

	v.animation_duration = 5;
	v.animation_speed = 0.2;
	v.animation_framerate = 24;
	v.time_speed = v.animation_speed;

	v.max_samples = 1;
	v.samples_per_step = 1;
	v.max_depth = 2;
	v.animation_render_mode = RAYTRACE_STEPS_3;
	v.irradiance_map = from_bmp("emptyroom1k.bmp");

	v.lights = malloc(2*sizeof(t_light));
	v.light_count = 0;
	v.lights[0] = (t_light){v3(1,.2,.4),
							v3(-1, .5, .3),
							v3(),
							20.0,
							False};
	vec3	*lp = &v.lights[0].pos;
	double d = 1;
	//add_motion(&(lp->x), -d, d, sin_tween);
	//add_motion(&(lp->z), -d, d, cos_tween);
	double	*str = &v.items[1].mat.normal_strength;
	*str = .08;
	double	*rot_y = &v.items[1].rot.y;
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
	v.background_color = shit_sky_background;
	v.sunIntensity = .2;
	v.sunFocus = 30;
	v.sunDirection = v3(.5, .5, -1);
	v.skyColorZenith = v3(.4, .9, .9);

	material ground = new_lambertian(checkerboard(.5,
		c3(.9, .5, .1),//checkerboard(1, c3(.1,.1,.1), c3(.9, .9, .9)),
		c3(.6, .6, .6)));
	ground.roughness = BW_MAP(.02);
	texture shaaapes = from_bmp("shapes.bmp");
	ground.normal = checkerboard(.5,
		shaaapes,//checkerboard(1, c3(.1,.1,.1), c3(.9, .9, .9)),
		shaaapes);
	ground.normal_strength = -.5;
	ground.metalic = BW_MAP(.8);
	ground.specular = BW_MAP(0.0);
	material sphere = new_lambertian(from_bmp("earthmap1k.bmp"));
	sphere.normal = from_bmp("earthnormal1k.bmp");
	sphere.specular = BW_MAP(0.0);
	sphere.roughness = BW_MAP(1.0);

    add_item((t_item){v3( 0, 0, 0),	v_3(1),	v3(), ground, PLANE});
    add_item((t_item){v3( 0, 1.2, 0),		v_3(1),	v3(), sphere, SPHERE});
	(void) sphere;
	//add_motion(&(v.camera_pos.x), 4.5, -4.5, sin_tween);
	//add_motion(&(v.camera_pos.z), 4.5, -4.5, cos_tween);
	//add_motion(&(v.sunDirection))

	//Animation
	v.animation_duration = 2;
	v.animation_framerate = 24;
	v.animation_speed = .5;
	v.time_speed = 0.5;

	v.lights = malloc(2*sizeof(t_light));
	v.light_count = 0;
	v.lights[0] = (t_light){v3(1,.8,.4),
							v3(-10, 1.2, 0),
							v3(),
							200.0,
							False};
	v.max_depth = 3;
	v.max_samples = 5;

	vec3	*lp = &v.lights[0].pos;
	double d = 10;
	add_motion(&(lp->x), -d, d, sin_tween);
	add_motion(&(lp->z), -d, d, cos_tween);
}

void	pbr_balls(void)
{
	default_cam();
	v.background_color = black_background;
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
		add_item((t_item){v3((count/2 - i)*1.5, _y, 0),	v_3(.5),v3(), m, SPHERE});
	}
	_y -= 1.5;

	m.base_color = c3(1,0,0); //red
	m.metalic = NO_MAP;
	m.roughness = BW_MAP(0.);
	for (double i=0; i<=count; i++)
	{
    	m.specular = BW_MAP(0.1 * i);
		add_item((t_item){v3((count/2 - i)*1.5, _y, 0),	v_3(.5),v3(), m, SPHERE});
	}
	_y -= 1.5;

	m.base_color = c3(.6,.6,.8); //blueish
	m.metalic  = NO_MAP;
	m.specular = NO_MAP;
	for (double i=0; i<=count; i++)
	{
    	m.roughness = BW_MAP(0.1 * i);
		add_item((t_item){v3((count/2 - i)*1.5, _y, 0),	v_3(.5),v3(), m, SPHERE});
	}
	_y -= 2.5;

	m.base_color = c3(139.0/256.0,173.0/256.0,158.0/256.0); //greenish
	m.metalic  = NO_MAP;
	m.specular = NO_MAP;
	m.roughness= NO_MAP;
	for (double i=0; i<=count; i++)
	{
		m.transmission = 0.1 * i;
		add_item((t_item){v3((count/2 - i)*1.5, _y, 0),	v_3(.5),v3(), m, SPHERE});
	}
	_y -= 2.5;

	m.transmission = 1.0;
	for (double i=0; i<=count; i++)
	{
		m.transmission_roughness = BW_MAP(0.1 * i);
		add_item((t_item){v3((count/2 - i)*1.5, _y, 0),	v_3(.5),v3(), m, SPHERE});
	}

	material m2 = new_lambertian(c3(1,1,1));
	//m2.metalic = BW_MAP(1.0);
	m2.roughness = BW_MAP(0*1.0);
	m2.specular = BW_MAP(1.0);
	m2.ior = 10;
	add_item((t_item){v3(),	v_3(.5),v3(), m2, SPHERE});
	material check = new_lambertian(checkerboard(.05, c3(.3,.3,.3), c3(.5,.5,.5)));
//	add_item((t_item){v3(0,-.5),v_3(10),v3(), check, QUAD});
	material l = new_lambertian(c3(1,1,1));
	material r = new_lambertian(c3(.5,.6,1));
//	add_item((t_item){v3(-1.3),	v_3(.5),v3(), l, SPHERE});
//	add_item((t_item){v3(1.3),	v_3(.5),v3(), r, SPHERE});

	v.vfov = 0.1;
	v.w = 300;
	v.h = 300;
	// v.camera_pos = v3(0.252365, 0.65293, 2.384792);
	// v.vfov = 50;
	v.max_depth = 2;
	v.max_samples = 1;
	v.render_mode = RAYTRACE_STEPS;
	v.irradiance_map = from_bmp("emptyroom1k.bmp");//BW_MAP(0.5);

	v.lights = malloc(sizeof(t_light));
	v.light_count = 0;
	v.lights[0] = (t_light){WHITE,
						v3(-1,0,1),
						v3(-1,-1,-1),
						2.0,
						True};
}

//__NOT__
void    shapes(void)
{
    default_cam();
	v.camera_pos = v3(-1.743579, 1.213285, 9.192838);

    
	add_item((t_item){	v3(3, 3), v_3(1), 	v3(), new_lambertian(c3(.2, 0, 1)),		SPHERE});
	add_item((t_item){	v3(3, 0), v_3(1), 	v3(), new_lambertian(c3(.2, 1, .4)),	CYLINDER});
	add_item((t_item){	v3(-3, 3), v_3(1),	v3(), new_lambertian(c3(0, .6, .9)),	PYRAMID});
    add_item((t_item){	v3(-3, 0), v_3(1),	v3(), new_lambertian(c3(1, .35, .1)),	BOX});
	add_item((t_item){	v3(-6, 0),v3(1.5,1,1), v3(), new_lambertian(c3(.6, .8, .2)),BOX});
}
