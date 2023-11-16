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

	PFPN _w = 1;
	PFPN _h = 1;
	PFPN _d = 1;
	
	PFPN s = 0.4;

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
#endif
