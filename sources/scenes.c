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
void	lights(void);
void	cornell(void);
void	checkers(void);
void	first_animation(void);

void    shapes(void);
//.. world, specular, glass, reflection...

void    init_scene(void)
{
    switch (7)
    {
        case 1 : balls(); break;
        case 2 : tweens(); break;
        case 3 : quads(); break;
		case 4 : lights(); break; 
		case 5 : cornell(); break;
		case 6 : checkers(); break;
		case 7 : first_animation(); break;

        case 10 : shapes(); break;
    }
}

void    default_cam(void)
{
	v.camera_pos = v3(0, 0, 3);
	v.camera_rot = v3(0, MYPI, 0);
	v.lookat_toggle = 0;
	v.lookat = v3(0, 0, 0);
	v.vup = v3(0,1,0);
	v.near_plane = NEAR;
	v.far_plane = FAR;
	v.vfov = 50;

	v.orthographic_toggle = 0;
	v.render_mode = RAYTRACE;

	v.background_color = sky_background;
	v.time_speed = 1;
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
    material material_right  = new_metal(c3(0.8, 0.6, 0.2), 0.1);

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

	add_motion(&(v.items[ 9].mat.albedo.color_value.z), 0, 255, lerpd);
	add_motion(&(v.items[10].mat.albedo.color_value.z), 0, 255, ping_pong);
	add_motion(&(v.items[11].mat.albedo.color_value.z), 0, 255, ping_pong_2);
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

void	lights(void)
{
	default_cam();
    v.render_mode = RAYTRACE;
	v.camera_pos = v3(-1.518677, 1.338787, 2.834727);
	v.lookat_toggle = 1;
	v.lookat = v3(0, 1, 0);
	v.background_color = black_background;

    material material_ground = new_lambertian(c3(.7,.8,.9));
    add_item((t_item){v3(0), v_3(100),	v3(), material_ground, PLANE});

	material light = new_light(c3(40, 40, 40));
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

void    cornell(void)
{
    default_cam();
    v.render_mode = RAYTRACE;
	v.camera_pos = v3(0, 0, 3.5);
	v.background_color = black_background;

    material material_wall = new_lambertian(c3(.73, .73, .73));
	
	material material_wall_left  = new_lambertian(c3(.12, .45, .15));
	material material_wall_bot   = material_wall;
	material material_wall_right = new_lambertian(c3(.65, .05, .05));

	double _w = 1;
	double _h = 1;
	double _d = 1;
	
	double s = 0;

	add_item((t_item){v3( 0, _h+s, 0),	v3(_w, 1, _d),	v3(MYPI,0,0), 	material_wall,		QUAD});
	add_item((t_item){v3( 0, 0,-_d-s),	v3(_w, 1, _h),	v3(MYPI/2,0,0), material_wall,		QUAD});
	add_item((t_item){v3( 0, 0,_d-s),	v3(_w, 1, _h),	v3(-MYPI/2,0,0),material_wall,		SS_QUAD});
	
	add_item((t_item){v3( _w+s, 0, 0),	v3(_h, 1, _d),	v3(0,0,MYPI/2),	material_wall_left,	QUAD});
	add_item((t_item){v3( 0,-_h-s, 0),	v3(_w, 1, _d),	v3(0,0,0), 		material_wall_bot, 	QUAD});
	add_item((t_item){v3(-_w-s, 0, 0),	v3(_h, 1, _d),	v3(0,0,-MYPI/2),material_wall_right,QUAD});

	material light = new_light(checkerboard(0.2, c3(.1,.1,.1), c3(3,3,3)));
	add_item((t_item){v3( 0, _h+s-1e-8, 0),	v_3(.8),	v3(MYPI,0,0), 	light,	QUAD});

	material mirror = new_metal(c3(1,1,1), 0);
	add_item((t_item){v3( 0, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE});
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
