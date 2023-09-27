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
void	cornell(void);
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

void    shapes(void);
//.. world, specular, glass, reflection...

void    init_scene(void)
{
	v.item_count = 0;
	v.items = NULL;

	v.motion_count = 0;
	v.motions = NULL;

    switch (11)
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
		case 9 : specular(); break;
		case 10: earth(); break;
		case 11: bumpy(); break;
		case 12: mirrors(); break;
		case 13: tomato(); break;
		case 14: glass_ball(); break;
		//point lights
		//normal_bump
		//specular
		//directional_ligth

        case 99 : shapes(); break;
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
	v.render_mode = RAYTRACE;

	v.background_color = sky_background;
	v.time_speed = 1;

	v.max_depth = 8;

	v.uv_debug = from_bmp("uv_check.bmp");

	v.max_samples = 50;
	v.samples_per_step = 10;

	v.animation_duration = 1.0;
	v.animation_framerate = 24;
	v.animation_speed = 1.0;
}

void    balls(void)
{
	default_cam();
    v.render_mode = RAYTRACE;
	v.lookat_toggle = 1;
	v.lookat = v3(0,0,0);
	v.camera_pos = v3(2.16, 1.45, 1.57);
	v.vfov = 20;

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
//light
void    cornell(void)
{
    default_cam();
    v.render_mode = RAYTRACE;
	v.camera_pos = v3(0, 0, 3.2);
	v.background_color = black_background;
	v.lookat = v3(0, 0, 0);
	v.lookat_toggle = 1;

    material material_wall = new_lambertian(c3(.73, .73, .73));
	
	material material_wall_left  = new_lambertian(c3(.12, .45, .15));
	material material_wall_bot   = new_lambertian(checkerboard(.2, c3(.1, .2, .8), c3(.3, .4, .8)));
	material material_wall_right = new_lambertian(c3(.65, .05, .05));

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

	material light = new_light(WHITE_MAP, 3);//checkerboard(0.2, c3(.1,.1,.1), c3(3,3,3)));
	add_item((t_item){v3( 0, _h+s-1e-8, 0),	v_3(.6),	v3(MYPI,0,0), 	light,	SS_QUAD});
}

void	mirror_balls(void)
{
	cornell();
	
	material mirror = new_metal(c3(1,1,1), 0);
	add_item((t_item){v3( -1, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE});
	add_item((t_item){v3( 1, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE});

	t_item *sph1 = &(v.items[7]);
	t_item *sph2 = &(v.items[8]);
	add_motion(&(sph1->pos.x), -1, -.5, easeInOutCubic);
	add_motion(&(sph2->pos.x), 1, .5, easeInOutCubic);
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
    add_item((t_item){v3( -_s, _h, -_s),	v_3(.5),v3(), light_red	, SPHERE});
    add_item((t_item){v3(  _s, _h, -_s),	v_3(.5),v3(), light_gre	, SPHERE});
    add_item((t_item){v3( -_s, _h,  _s),	v_3(.5),v3(), light_blu	, SPHERE});

	v.motion_enabled = 1;
	v.time_speed = 0.2;
	add_motion(&(v.camera_pos.x), -4, 4, sin_tween);
	add_motion(&(v.camera_pos.z), -4, 4, cos_tween);

	v.upscale = 3;
	v.max_depth = 50;
	v.motion_enabled = 0;
	v.render_mode = RASTER;
}

void	specular(void)
{
	cornell();
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

void	tomato(void)
{
	cornell();
	v.camera_pos = v3 (0.000000, -0.080084, 2.572990);
	v.lookat_toggle = 1;
	v.max_depth = 10;
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

	material light = new_light(WHITE_MAP, 5.0);

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
	v.camera_pos = v3(0, 0, 4.5);
	v.lookat_toggle = 1;
	v.background_color = shit_sky_background;

	texture earth_texture = from_bmp("earth.bmp");
	material earth_material = new_lambertian(earth_texture);

	texture check = checkerboard(.25, c3(.1,.1,.1), c3(.9,.9,.9));
	material ground = new_lambertian(checkerboard(.5,
		earth_texture,
		c3(.9, .9, .9)));

	add_item((t_item){v3( 0, 0, 0),	v_3(1),	v3(), 	earth_material,	SPHERE});
	add_item((t_item){v3( 0, -1, 0),v3(2,1,1),	v3(), 	ground,	PLANE});
}

//UV test

void	bumpy(void)
{
	default_cam();
    v.render_mode = RAYTRACE_STEPS;
	v.camera_pos = v3(0, 0, 2.25);
	v.lookat_toggle = 1;
	v.max_depth = 2;
	v.upscale = 1;
	v.w = 512;
	v.h = 512;
	v.background_color = black_background;
	v.max_samples = 1;

	material light = new_light(WHITE_MAP, 10.0);//checkerboard(0.2, c3(.1,.1,.1), c3(3,3,3)));
	light.base_color = c3(.1,.1,.1);
	add_item((t_item){v3( 0, 0, 4),	v_3(.8),	v3(-MYPI/2,0,0), 	light,	SS_QUAD});

	texture ea = WHITE_MAP;//from_bmp("earthmap1k.bmp");
	texture ea_bump = WHITE_MAP;//from_bmp("shapes.bmp");
	material earth_bumpy = new_lambertian_bump(ea,ea_bump);
	//earth_bumpy.specular = from_bmp("eartspec1k.bmp");
	earth_bumpy.roughness = NO_MAP;

	add_item((t_item){v3( 0, 0, 0),	v_3(1),	v3(), 	earth_bumpy,	PLANE});


	t_item *sph = &(v.items[1]);
	add_motion(&(sph->rot.y), 0, -MYPI*2, lerpd);
	add_motion(&(sph->rot.x), 0, -MYPI*2, lerpd);
	//add_motion(&(v.camera_pos.x), -1.5, 1.5, sin_tween);
	//add_motion(&(v.camera_pos.z), -1.5, 1.5, cos_tween);
	v.time_speed = 0.2;
	v.animation_duration = 5;
	v.animation_speed = 0.2;
	v.animation_framerate = 10;

	v.max_samples = 10;
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

	double fuzzzz = 0.01;

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
	add_item((t_item){v3( 0, _h+s-1e-8, 0),	v_3(.8),	v3(MYPI,0,0), 	light,	SS_QUAD});

	material mirror = new_metal(c3(1,1,1), fuzzzz);
	add_item((t_item){v3( 0, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE});
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
