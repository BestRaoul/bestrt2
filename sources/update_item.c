/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upate_item.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

enum e_et {
	SET = 0,
	UPDATE,
	CONFIRM,
	RESET
};

void	default_crusor()
{
	set_cursor(XC_crosshair);
}

int	set_reset(tfm *transform, vec3 *inital_mp, vec3 *init_p, int mode, int et)
{
	if (et == SET)
	{
		if (mode == MOVE)	*init_p = transform->pos;
		if (mode == ROTATE)	*init_p = transform->rot;
		if (mode == SCALE)	*init_p = transform->scale;
		*inital_mp = v.mouse_pos;
		v.selection_mode = mode;
		v.render_mode = RASTER;

		if (mode == MOVE)	set_cursor(XC_fleur);
		if (mode == ROTATE)	set_cursor(XC_exchange);
		if (mode == SCALE)	set_cursor(XC_sizing);

		return 1;
	}
	if (et == RESET)
	{
		if (mode == MOVE)	transform->pos = *init_p;
		if (mode == ROTATE)	transform->rot = *init_p;
		if (mode == SCALE)	transform->scale = *init_p;
		default_crusor();
		return 1;
	}
	return 0;
}

double distanceToLine(vec3 A, vec3 B, vec3 C) {
    double numerator = fabs((B.x - A.x) * (A.y - C.y) - (A.x - C.x) * (B.y - A.y));
    double denominator = sqrt((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));
    double distance = numerator / denominator;
    return distance;
}

vec3 projectOntoLine(vec3 A, vec3 B, vec3 C) {
	vec3 AB, AC, P;
    double dotAB, dotAC;

    // Calculate vector AB
    AB.x = B.x - A.x;
    AB.y = B.y - A.y;

    // Calculate vector AC
    AC.x = C.x - A.x;
    AC.y = C.y - A.y;

    // Calculate dot products
    dotAB = AB.x * AB.x + AB.y * AB.y;
    dotAC = AC.x * AB.x + AC.y * AB.y;

    // Calculate the projected vector P
    P.x = (dotAC / dotAB) * AB.x;
    P.y = (dotAC / dotAB) * AB.y;

    return v_add(P, A);
}

double directioned_distance(vec3 from, vec3 to, vec3 dir)
{
	double d = vec_dist(from, to);

	vec3 my_dir = from_to(from, to);
	if (signd(my_dir.x) != signd(dir.x) && signd(my_dir.y) != signd(dir.y))
		return -d;
	return d;
}

Bool	smth_smth_check(vec3 nmp, vec3 pp, vec3 A, vec3 B, vec3 C, vec3 D)
{
	double dtp = vec_dist(nmp, pp);
	vec3 I1 = line_intersection(nmp, pp, A, B);
	vec3 I2 = line_intersection(nmp, pp, C, D);
	double dt1 =  directioned_distance(nmp, I1, from_to(nmp, pp));
	double dt2 =  directioned_distance(nmp, I2, from_to(nmp, pp));

	if (dt1 < 0) dt1 = INFINITY;
	if (dt2 < 0) dt2 = INFINITY;

	return (dtp < dt1 && dtp < dt2);
}

double	tddd(vec3 from, vec3 to, vec3 dir)
{
	vec3 v = from_to(from, to);
	double d = sqrt(v.x*v.x + v.y*v.y);
	if (v_dot(v, dir) < 0)
		return -d;
	return d;
}

//in Radians
//[0, MYPI]
double get_angle(vec3 dir)
{
	double a = atan2(dir.y, dir.x);
	if (a < 0)
		return a + MYPI;
	return a;	
}

int	get_move_plane(vec3 mp, ri r)
{
	double a_m =  get_angle(from_to(r.anchor, mp));
	double a_r =  get_angle(from_to(r.anchor, r.r_top));
	double a_g =  get_angle(from_to(r.anchor, r.g_top));
	double a_b =  get_angle(from_to(r.anchor, r.b_top));

	double up_bound = 10;
	double low_bound = -10;
	if (a_r < up_bound && a_r > a_m) up_bound = a_r;
	if (a_r > low_bound && a_r < a_m) low_bound = a_r;
	if (a_g < up_bound && a_g > a_m) up_bound = a_g;
	if (a_g > low_bound && a_g < a_m) low_bound = a_g;
	if (a_b < up_bound && a_b > a_m) up_bound = a_b;
	if (a_b > low_bound && a_b < a_m) low_bound = a_b;

	if (up_bound  ==  10) up_bound = fmin(fmin(a_r, a_g), a_b);
	if (low_bound == -10) low_bound = fmax(fmax(a_r, a_g), a_b);

	if (a_r != up_bound && a_r != low_bound) return YZ;
	if (a_g != up_bound && a_g != low_bound) return XZ;
	if (a_b != up_bound && a_b != low_bound) return XY;
	write(1, "WTF!", 4);

	// int mv_plane = get_move_plane(mp, r);
	// if (v.plane == XY || v.plane == XZ || v.plane == YZ) mv_plane = v.plane;
	// (void) mv_plane;
	return X;
}

//should be on top
void	draw_modify_axis(ri r, vec3 inital_position)
{
	if (v.selection_mode != NONE && v.plane != XYZ)
	{
		if (X_ENABLED)
		{
			vec3 f = v_add(inital_position, v3( v.far));
			vec3 t = v_add(inital_position, v3(-v.far));
			draw_projected_line(f, t, X_COLOR);
		}
		if (Y_ENABLED)
		{
			vec3 f = v_add(inital_position, v3(0, v.far));
			vec3 t = v_add(inital_position, v3(0,-v.far));
			draw_projected_line(f, t, Y_COLOR);
		}
		if (Z_ENABLED) {
			vec3 f = v_add(inital_position, v3(0,0, v.far));
			vec3 t = v_add(inital_position, v3(0,0,-v.far));
			draw_projected_line(f, t, Z_COLOR);
		}
	}
}

void	get_intersections(vec3 pos, vec3 mp, vec3 *ir, vec3 *ib)
{
	vec3 anchor = world_to_screenpos(pos);
	t_item temp = (t_item){(tfm){pos, v3(), v_3(1)}, new_m(BLACK), NULL, hit_plane};
	hit_record rec;

	//X - Z
	set_transform_matrix(&temp.transform, temp.fwd, temp.bck);
	ray raii; init_ray(mp.x,mp.y,&raii);
	if (hit_plane(&raii, INTERVAL_FORWARD, &rec, &temp))
	{
		*ir = v3(rec.p.x, pos.y, pos.z);
		*ib = v3(pos.x, pos.y, rec.p.z);
	}
	else
	{
		*ir = pos;
		*ib = pos;
	}
}

void	v_round(vec3 *in, double precision)
{
	in->x = (int)(in->x/precision) * precision;
	in->y = (int)(in->y/precision) * precision;
	in->z = (int)(in->z/precision) * precision;
}

void	move_transform(tfm *transform, int et)
{
	static vec3 initial_position = (vec3){};
	static vec3 pmp = (vec3){};
	vec3 mp = v.mouse_pos;

	if (set_reset(transform, &pmp, &initial_position, MOVE, et))
		return;
	ri r = get_rotation_indicator(initial_position, world_to_screenpos(initial_position));
	draw_modify_axis(r, initial_position);

	vec3	pg = projectOntoLine(r.g_bot, r.g_top, mp);
	pg.z = 0;
	double dy = tddd(r.anchor, r.g_top, from_to(r.anchor, r.g_top));

	vec3 ir, ib;
	get_intersections(initial_position, mp, &ir, &ib);

	vec3 move = v3(0,0,0);
	move.x = ir.x - initial_position.x;
	move.z = ib.z - initial_position.z;
	move.y = tddd(r.anchor, pg, from_to(r.anchor, r.g_top)) / dy;

	if (v_eq(pmp, mp))
		return;

	transform->pos = plane_alligned_add(initial_position, move);
	if (v._ctrl) v_round(&transform->pos, 0.25);

	pmp = mp;
}

void	scale_transform(tfm *transform, int et)
{
	static vec3 initial_scale = (vec3){};
	static vec3 inital_mp = (vec3){};
	vec3 mp = v.mouse_pos;

	if (set_reset(transform, &inital_mp, &initial_scale, SCALE, et))
		return;
	ri r = get_rotation_indicator(transform->pos, world_to_screenpos(transform->pos));
	draw_modify_axis(r, transform->pos);

	double initial_d = vec_dist(r.anchor, inital_mp);
	double d = vec_dist(r.anchor, mp);

	vec3 move = v_scal(initial_scale, d / initial_d);
	move = v_sub(move, initial_scale);

	gizmo_line(r.anchor, mp, WHITE);

	transform->scale = plane_alligned_add(initial_scale, move);
	if (v._ctrl) v_round(&transform->scale, 0.1);
}

void	rotate_transform(tfm *transform, int et)
{
	static vec3 initial_rot = (vec3){};
	static vec3 inital_mp = (vec3){};
	vec3 mp = v.mouse_pos;

	if (set_reset(transform, &inital_mp, &initial_rot, ROTATE, et))
		return;
	ri r = get_rotation_indicator(transform->pos, world_to_screenpos(transform->pos));
	draw_modify_axis(r, transform->pos);

	vec3 initial_dir = from_to(r.anchor, inital_mp);
	vec3 dir = from_to(r.anchor, mp);
	double initial_angle = atan2(initial_dir.y, initial_dir.x) + MYPI;
	double angle = atan2(dir.y, dir.x) + MYPI;
	
	vec3 move = v_3(angle - initial_angle);

	gizmo_line(r.anchor, mp, WHITE);

	transform->rot = plane_alligned_add(initial_rot, move);
	if (v._ctrl) v_round(&transform->rot, 5*DEG2RAD);
}

void	mrs_null_reference(tfm *_, int et)
{
	printf("ERROR in MRS event :: null reference\n");
}

void (*apt[4])(tfm *, int) = {
	mrs_null_reference,
	move_transform,
	rotate_transform,
	scale_transform
};

void	manage_selection(void)
{
	if (v._lclick)
	{
		v._lclick = 0;
		default_crusor();
		if (v.selected && v.selection_mode != NONE)
		{
			v.selection_mode = NONE;
			return;
		}
		v.selection_mode = NONE;

		ray r; hit_record rec;
		init_ray(v.mouse_pos.x, v.mouse_pos.y, &r);
		if (info_hit(&r, (interval){0, INFINITY}, &rec))
		{
			if (v.selected == &rec.item->transform)
			{
				v.lookat_toggle = 1; v.camera_change = 1; 
			}
			v.selected = &rec.item->transform;
			v.render_mode = RASTER;
			v.selection_mode = NONE;

			v.lookat = v.selected->pos;
			v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
		}
		else
			v.selected = NULL;
	}

	if (v.selected == NULL) return;

	if (v.selection_mode == NONE) // unselect
	{
		if (v._rclick)
		{
			v.selected = NULL;
			v._rclick = 0;
			default_crusor();
		}
		return;
	}

	if (v._space) //Confirm
	{
		v.selection_mode = NONE;
		v._space = 0;
		default_crusor();
		return;
	}
	if (v._rclick) //Reset
	{
		v._rclick = 0;
		apt[v.selection_mode](v.selected, RESET);
		v.selection_mode = NONE;
		default_crusor();
		return;
	}

	apt[v.selection_mode](v.selected, UPDATE);
}

void	maybe_add_item(void)
{
	if (v.selected != NULL) return;

	//item
	if (v._1 || v._2 || v._3 || v._4)
	{
		t_item *new_item = add_item_(get_item_default());
		v.selected = &new_item->transform;
		v.selection_is_item_lamp = True;
		
		move_transform(v.selected, SET);
		v.lookat = v.selected->pos; v.lookat_toggle = 1;
		v.camera_change = 1; v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
	}
	//lamp
	//lamp 6 (directional) : overides selection mode to rotate
	if (v._5 || v._6)
	{
		t_light *new_lamp = add_lamp_(get_lamp_default());
		v.selected = &new_lamp->transform;
		v.selection_is_item_lamp = False;

		move_transform(v.selected, SET);
		v.lookat = v.selected->pos; v.lookat_toggle = 1;
		v.camera_change = 1; v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
	}
return;

// static vec3		base = (vec3){0};
// static vec3		anchor = (vec3){0};
// static vec3		total_move = (vec3){0};
// static vec3		mouse_prev = (vec3){0};

// 	if (v._space)
// 	{
// 		v._space = 0;
// 		total_move = v3(0);
// 		v._lclick = 1;
// 	}
// 	if (v._rclick)
// 	{
// 		remove_item(addin);
// 		v.selection_mode = NONE;
// 		addin = NULL;
// 		v._rclick = 0;
// 		return;
// 	}
	
// 	vec3 move = v3(v.mouse_sc->scale.x - mouse_prev.x, v.mouse_pos.y - mouse_prev.y);
// //	if (v._shift) move = v3(move.x/10, move.y/10};
// 	total_move = v3(total_move.x + move.x, total_move.y + move.y);
// 	v.selection_pos = addin->pos;
// 	vec3 selection_point = world_to_screenpos(addin->pos);

// 	if (stage == 0) //Init
// 	{
// 		stage = 1, total_move = v3(0);
// 		base = v3(0,0,0);
// 		anchor = v.mouse_pos;
// 		mouse_prev = v.mouse_pos;
// 		v.selection_mode = MOVE;
// 	}
// 	if (stage == 1) //Position
// 	{
// 		double aspect = (double)v.w/(double)v.h;
// 		vec3 uv = v3((v.mouse_pos.x)/v.h, (v.h-v.mouse_pos.y)/v.h, .2);
// 		vec3 uv2= v_scal(v_add(uv, v3(-.5*aspect,-.5)), 3);
// 		vec3 pos = reverse_project(uv2, v.vfov, 1);
// 		pos = rotate3(pos, v.camera_rot);
// 		pos = v3(total_move.x*3.0/v.w, -total_move.y*3.0/v.h, pos.x+pos.y);
// 		addin->pos = plane_alligned_add(base, v_scal(pos, 4));
// 		if (v._lclick)
// 		{
// 			stage = 2, v._lclick = 0, total_move = v3(0);
// 			base = addin->scale;
// 			anchor = v3(vec_dist(selection_point, v.mouse_pos));
// 			v.selection_mode = SCALE;
// 		}
// 	}
// 	if (stage == 2) //Scale
// 	{
// 		double d = vec_dist(selection_point, v.mouse_pos);
// 		addin->scale = plane_alligned_add(base, v_scal(v_3(d - anchor.x), 10.0/v.h));
// 		if (v._lclick)
// 		{
// 			stage = 3, v._lclick = 0, total_move = v3(0);
// 			base = addin->rot;
// 			anchor = v.mouse_pos;
// 			v.selection_mode = ROTATE;
// 		}
// 	}
// 	if (stage == 3) //Rotate
// 	{
// 		vec3 g = v3(anchor.x-selection_point.x, anchor.y-selection_point.y);
// 		double start_angle = atan2(g.y, g.x);
// 		double end_angle = atan2(g.y + total_move.y, g.x + total_move.x);
// 		//if (sign(v.mouse_pos.x-addin->pos.x) != sign(anchor.x-addin->pos.x)) end_angle += MYPI;
// 		addin->rot = plane_alligned_add(base, v3(-start_angle+end_angle, -start_angle+end_angle, -start_angle+end_angle));
// 		if (v._lclick)
// 		{
// 			stage = 5, v._lclick = 0;
// 			v.selection_mode = NONE;
// 		}
// 	}	
// 	//4 //pick color
// 	if (stage == 5) //forget the added one
// 	{
// 		addin = NULL;
// 		v.selection_mode = NONE;
// 	}
	
// 	mouse_prev = v.mouse_pos;
}
