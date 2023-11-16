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


	PFPN _d = 0;
	PFPN _r = 0;
	PFPN _beta = 0;
	vec3 pp = v3();
	for (int i=0; i<50; i++)
	{
		material m = new_lambertian(c3(1, random_pfpn(), random_pfpn()));
		vec3	p = rotate_y(v3(_d, 0, 0), _beta);
    	PFPN radius = vec_dist(p,pp)/2;
		p.y += radius;
		add_item(p, v_3(radius), v3(), m, SPHERE);
		_d += 0.5;
		_beta += 15*DEG2RAD;
		pp = p;
	}

}

void	mirror_balls(void)
{
	cornell(v3(0));

	
	material mirror = new_metal(c3(1,1,1), 0);
	add_item(v3( -1, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE);
	add_item(v3( 1, -.5, 0),	v_3(.5),	v3(), 	mirror,	SPHERE);

	t_item *sph1 = &(v.items[7]);
	t_item *sph2 = &(v.items[8]);
	add_motion(&(sph1->transform.pos.x), -1, -.5, ease_in_out_cubic);
	add_motion(&(sph2->transform.pos.x), 1, .5, ease_in_out_cubic);

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

	PFPN _h = 3;
	PFPN _s = 1;
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

#endif
