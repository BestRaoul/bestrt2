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
	v.animation_render_mode = RAYTRACE_STEPS;
	v.irradiance_map = from_bmp("textures/emptyroom1k.bmp");

	add_lamp(v3(1,.2,.4), v3(-1, .5, .3), 20.0, False);
	vec3	*lp = &v.lights[0].transform.pos;
	PFPN d = 1;
	//add_motion(&(lp->x), -d, d, sin_tween);
	//add_motion(&(lp->z), -d, d, cos_tween);
	PFPN	*rot_y = &v.items[1].transform.rot.y;
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
	PFPN d = 10;
	add_motion(&(lp->x), -d, d, sin_tween);
	add_motion(&(lp->z), -d, d, cos_tween);
}

#endif
