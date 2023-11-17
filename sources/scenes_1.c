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

void	default_cam(void)
{
	v.w = 16 * (2 * 2 * 3 * 5);
	v.h = 9 * (2 * 2 * 3 * 5);
	(seti(&v.upscale, 3), seti(&v.lookat_toggle, 0));
	v.camera_pos = v3(0, 0, 3);
	v.camera_rot = v3(0, MYPI, 0);
	v.lookat = v3(0, 0, 0);
	v.vup = v3(0, 1, 0);
	(set(&v.near, 0.2), set(&v.far, 1000), set(&v.vfov, 50));
	v.render_mode = RAYTRACE_STEPS;
	v.orthographic_toggle = 0;
	(set(&v.defocus_angle, 0), set(&v.focus_dist, 10));
	(set(&v.time_speed, 1));
	(seti(&v.max_depth, 6), seti(&v.max_samples, 10));
	seti(&v.samples_per_step, 5);
	(set(&v.animation_duration, 1.0), set(&v.animation_speed, 1.0));
	v.animation_framerate = 24;
	v.animation_render_mode = RAYTRACE;
	v.animation_loops = 0;
	v.rendering_movie = False;
	(setb(&v.use_background, True), setb(&v.use_ibl, False));
	v.background = cc3(WHITE);
	v.irradiance_map = NO_MAP;
	v.blurry_irradiance_map = NO_MAP;
	v.uv_debug = from_bmp("textures/debug/uv_check.bmp");
	v.solo_lighting = 0;
}

#ifndef ILLEGAL

void	init_scene(int select)
{
	notify_illegal("premade scene");
	my_exit();
	(void)select;
}
#endif

#ifdef ILLEGAL

void	init_scene(int select)
{
	v.item_count = 0;
	v.light_count = 0;
	v.motion_count = 0;
	switch (select)
	{
	case 1:
		balls();
		break ;
	case 2:
		tweens();
		break ;
	case 3:
		quads();
		break ;
	case 0:
		lights0();
		break ;
	case 4:
		lights();
		break ;
	case -4:
		lights2();
		break ;
	case 5:
		mirror_balls();
		break ;
	case 6:
		checkers();
		break ;
	case 7:
		first_animation();
		break ;
	case 8:
		colored_lights();
		break ;
	case 9:
		specular();
		break ; // specular sucks
	case 10:
		earth();
		break ;
	case 11:
		bumpy();
		break ;
	case 12:
		mirrors();
		break ;
	case 13:
		tomato();
		break ;
	case 14:
		glass_ball();
		break ;
	case 15:
		normal_reflection();
		break ;
		// point lights
		// normal_bump
		// specular
		// directional_ligth
	case 101:
		showoff_1();
		break ;
	case 102:
		showoff_2();
		break ;
	case 103:
		showoff_3();
		break ;
	case 104:
		showoff_4();
		break ;
	case 105:
		showoff_5();
		break ;
	case 106:
		showoff_6();
		break ;
	case 107:
		showoff_7();
		break ;
	case 108:
		showoff_8();
		break ;
	case 109:
		showoff_9();
		break ;
	case 100:
		test();
		break ;
	default:
		write(2,
			"NO scene selected..\n1 : balls\n2 : tweens\n3 : quads\n0 : lights0\n4 : lights\n-4 : lights2\n5 : mirror_balls\n6 : checkers\n7 : first_animation\n8 : colored_lights\n9 : specular\n10: earth\n11: bumpy\n12: mirrors\n13: tomato\n14: glass_ball\n15: normal_reflection\n101: showoff_1\n102: showoff_2\n103: showoff_3\n104: showoff_4\n105: showoff_5\n106: showoff_6\n107: showoff_7\n108: showoff_8\n109: showoff_9\n",
			385);
		exit(1);
		break ;
	}
}

// top, back, front
// left, bot, right
// vec3(16 / 9, 1, 1)
void	cornell(vec3 dimensions)
{
	material	wall;
	material	wall_left;
	material	wall_bot;
	material	wall_right;
	PFPN		_w;
	PFPN		_h;
	PFPN		_d;
	PFPN		s;

	default_cam();
	v.camera_pos = v3(0, 0, MYPI);
	v.background = cc3(BLACK);
	v.irradiance_map = NO_MAP;
	v.lookat = v3(0, 0, 0);
	v.lookat_toggle = 1;
	wall = new_lambertian(c3(.73, .73, .73));
	wall_left = new_lambertian(c3(.12, .45, .15));
	wall_bot = new_lambertian(checkerboard(.2, c3(.1, .2, .8), c3(.3, .4, .8)));
	wall_right = new_lambertian(c3(.65, .05, .05));
	if (v_len(dimensions) == 0.0)
		dimensions = v3(16.0 / 9.0, 1, 1);
	_w = dimensions.x;
	_h = dimensions.y;
	_d = dimensions.z;
	s = 0;
	add_item(v3(0, _h + s, 0), v3(_w, 1, _d), v3(MYPI, 0, 0), wall, SS_QUAD);
	add_item(v3(0, 0, -_d - s), v3(_w, 1, _h), v3(MYPI / 2, 0, 0), wall,
		SS_QUAD);
	add_item(v3(0, 0, _d + s), v3(_w, 1, _h), v3(-MYPI / 2, 0, 0), wall,
		SS_QUAD);
	add_item(v3(_w + s, 0, 0), v3(_h, 1, _d), v3(0, 0, MYPI / 2), wall_left,
		SS_QUAD);
	add_item(v3(0, -_h - s, 0), v3(_w, 1, _d), v3(0, 0, 0), wall_bot, SS_QUAD);
	add_item(v3(-_w - s, 0, 0), v3(_h, 1, _d), v3(0, 0, -MYPI / 2), wall_right,
		SS_QUAD);
}

void	test(void)
{
	mirrors();
	return ;
}
#endif