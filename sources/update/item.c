
#include "fractol.h"

void	(*g_apt[4])(tfm *, int) = {
	mrs_null_reference,
	move_transform,
	rotate_transform,
	scale_transform};

static bool	shoot(void)
{
	ray			r;
	hit_record	rec;

	v._lclick = 0;
	default_crusor();
	if (v.selected && v.selection_mode != NONE)
		return (v.selection_mode = NONE, True);
	v.selection_mode = NONE;
	init_ray(v.mouse_pos.x, v.mouse_pos.y, &r);
	if (info_hit(&r, (interval){0, INFINITY}, &rec))
	{
		if (v.selected == &rec.item->transform)
		{
			v.lookat_toggle = 1;
			v.camera_change = 1;
		}
		v.selected = &rec.item->transform;
		v.render_mode = RASTER;
		v.selection_mode = NONE;
		v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
	}
	else
		v.selected = NULL;
	return (False);
}

static bool	unselect_confirm_or_reset(void)
{
	if (v.selection_mode == NONE)
	{
		if (v._rclick)
		{
			v.selected = NULL;
			v._rclick = 0;
			default_crusor();
		}
		return (True);
	}
	if (v._space)
	{
		v.selection_mode = NONE;
		v._space = 0;
		return (default_crusor(), True);
	}
	if (v._rclick)
	{
		v._rclick = 0;
		g_apt[v.selection_mode](v.selected, RESET);
		v.selection_mode = NONE;
		return (default_crusor(), True);
	}
	return (False);
}

void	manage_selection(void)
{
	if (v._lclick && shoot())
		return ;
	if (v.selected == NULL)
		return ;
	if (unselect_confirm_or_reset())
		return ;
	g_apt[v.selection_mode](v.selected, UPDATE);
}

void	maybe_add_item(void)
{
	t_item	*new_item;
	t_light	*new_lamp;

	if (v.selected != NULL)
		return ;
	if (v._1 || v._2 || v._3 || v._4)
	{
		new_item = add_item_(get_item_default());
		v.selected = &new_item->transform;
		v.selection_is_item_lamp = True;
		move_transform(v.selected, SET);
		v.camera_change = 1;
		v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
	}
	if (v._5 || v._6)
	{
		new_lamp = add_lamp_(get_lamp_default());
		v.selected = &new_lamp->transform;
		v.selection_is_item_lamp = False;
		move_transform(v.selected, SET);
		v.camera_change = 1;
		v.camera_dist_lookat = v_len(from_to(v.camera_pos, v.selected->pos));
	}
}

// if (v._np9)
// NOT_IMPLEMENTED("Flip around Y axis")
static void	rotate_camera_orbit_move(void)
{
	PFPN	deg;
	int		one;

	deg = 15 / (1 + 4 * v._shift);
	one = 1 - 2 * v.cam_flipp;
	if (v.lookat_toggle)
	{
		if (v._np4 || v._np6 || v._np2 || v._np8)
			do_orbit_move(v3(one * v._np8 - one * v._np2,
					one * v._np4 - one * v._np6), deg * DEG2RAD);
	}
	else
	{
		if (v._np4)
			v.camera_rot.y -= one * deg * DEG2RAD;
		if (v._np6)
			v.camera_rot.y += one * deg * DEG2RAD;
		if (v._np2)
			v.camera_rot.x += one * deg * DEG2RAD;
		if (v._np8)
			v.camera_rot.x -= one * deg * DEG2RAD;
	}
}

// orbit or fly rotation movement
void	rotate_camera(void)
{
	PFPN	dist_to;

	dist_to = vec_dist(v.camera_pos, v.lookat);
	if (v._np0)
	{
		v.camera_pos = v3(0, 0, 0);
		v.camera_rot = look_rotation(v3(), v3(0, 0, -1));
		v.lookat_toggle = 0;
	}
	if (v._np1 || v._np3 || v._np7)
	{
		v.camera_pos = v_mult(v_3(dist_to), v3(-v._np3, v._np7, v._np1));
		v.camera_pos.z += 1e-4 * v._np7;
		v.lookat_toggle = 1;
	}
	if (v._np5)
	{
		v.orthographic_toggle = !v.orthographic_toggle;
		v._rerender = 1;
	}
	rotate_camera_orbit_move();
}

static void	move_camera_nolook(void)
{
	vec3	move;
	int		one;
	PFPN	move_speed;

	one = 1 - 2 * v.cam_flipp;
	move_speed = get_move_speed();
	move = v3(0, 0, 0);
	if (v._right)
		move = v3(one);
	if (v._left)
		move = v3(-one);
	if (v._up)
		move = v3(0, 0, 1);
	if (v._down)
		move = v3(0, 0, -1);
	if (v._q)
		move = v3(0, one);
	if (v._e)
		move = v3(0, -one);
	move = v_scal(move, move_speed * v.delta_time);
	v.camera_pos = v_add(v.camera_pos, rotate3(move, v.camera_rot));
}

void	move_camera(void)
{
	PFPN	angular_speed;
	int		one;
	vec3	move;

	angular_speed = get_angular_speed();
	one = 1 - 2 * v.cam_flipp;
	if (v.lookat_toggle)
	{
		if (v._q)
			do_orbit_move(v3(one), angular_speed * v.delta_time);
		if (v._e)
			do_orbit_move(v3(-one), angular_speed * v.delta_time);
		if (v._right)
			do_orbit_move(v3(0, -1), angular_speed * v.delta_time);
		if (v._left)
			do_orbit_move(v3(0, 1), angular_speed * v.delta_time);
		if (v._up || v._down)
		{
			move = v3(0, 0, v._up - v._down);
			move = v_scal(move, angular_speed * v.delta_time);
			v.camera_pos = v_add(v.camera_pos, rotate3(move, v.camera_rot));
		}
	}
	else
		move_camera_nolook();
}

void	fov_camera(void)
{
	if (v._scroll)
		v.vfov = 50 + v._scroll;
}


void	maybe_flipp(void)
{
	static vec3	prev_pos = (vec3){};
	vec3		pp;
	vec3		p;

	if (v.lookat_toggle)
	{
		pp = v_sub(prev_pos, v.lookat);
		p = v_sub(v.camera_pos, v.lookat);
		if ((signd(pp.x) != signd(p.x) && signd(pp.z) != signd(p.z)))
			v.cam_flipp = !v.cam_flipp;
	}
	prev_pos = v.camera_pos;
}

PFPN	get_move_speed(void)
{
	const PFPN	move_acceleration = 3;
	const PFPN	top_speed = 100;
	static PFPN	s;

	if (v.lookat_toggle)
		s = 0;
	if (!v._right && !v._left && !v._up && !v._down && !v._q && !v._e)
		s = 0;
	s += move_acceleration * v.delta_time;
	s = clamp((interval){0, top_speed}, s);
	if (v._shift)
		return (s / 5);
	return (s);
}

PFPN	get_angular_speed(void)
{
	const PFPN	move_acceleration = MYPI / 3;
	const PFPN	top_speed = MYPI;
	static PFPN	s;

	if (!v.lookat_toggle)
		s = 0;
	if (!v._right && !v._left && !v._up && !v._down && !v._q && !v._e)
		s = 0;
	s += move_acceleration * v.delta_time;
	s = clamp((interval){0, top_speed}, s);
	if (v._shift)
		return (s / 5);
	return (s);
}

void	do_orbit_move(vec3 axis, PFPN angle)
{
	axis = rotate_y(axis, v.camera_rot.y);
	v.camera_pos = rotate_around(v.camera_pos, v.lookat, axis, angle);
}


typedef struct s_mt_data {
	vec3	pg;
	PFPN	dy;
	vec3	ir;
	vec3	ib;
}	t_mt_data;

void	move_transform(tfm *transform, int et)
{
	static vec3	initial_position = (vec3){};
	static vec3	pmp = (vec3){};
	ri			r;
	t_mt_data	data;

	if (set_reset(transform, &initial_position, MOVE, et))
	{
		pmp = v.mouse_pos;
		return ;
	}
	r = get_rotation_indicator(initial_position,
			world_to_screenpos(initial_position));
	draw_modify_axis(r, initial_position);
	data.pg = project_onto_line(r.g_bot, r.g_top, v.mouse_pos);
	data.pg.z = 0;
	data.dy = tddd(r.anchor, r.g_top, from_to(r.anchor, r.g_top));
	get_intersections(initial_position, v.mouse_pos, &data.ir, &data.ib);
	if (v_eq(pmp, v.mouse_pos))
		return ;
	transform->pos = plane_alligned_add(initial_position,
			v3(data.ir.x - initial_position.x,
				tddd(r.anchor, data.pg, from_to(r.anchor, r.g_top)) / data.dy,
				data.ib.z - initial_position.z));
	pmp = v.mouse_pos;
}
// if (v._ctrl) v_round(&transform->pos, 0.25);

void	scale_transform(tfm *transform, int et)
{
	static vec3	initial_scale = (vec3){};
	static vec3	inital_mp = (vec3){};
	ri			r;
	PFPN		initial_d;
	PFPN		d;

	if (set_reset(transform, &initial_scale, SCALE, et))
	{
		inital_mp = v.mouse_pos;
		return ;
	}
	r = get_rotation_indicator(transform->pos,
			world_to_screenpos(transform->pos));
	draw_modify_axis(r, transform->pos);
	initial_d = vec_dist(r.anchor, inital_mp);
	d = vec_dist(r.anchor, v.mouse_pos);
	gizmo_line(r.anchor, v.mouse_pos, WHITE);
	transform->scale = plane_alligned_add(initial_scale,
			v_sub(v_scal(initial_scale, d / initial_d), initial_scale));
}
// if (v._ctrl) v_round(&transform->scale, 0.1);

void	rotate_transform(tfm *transform, int et)
{
	static vec3	initial_rot = (vec3){};
	static vec3	inital_mp = (vec3){};
	ri			r;
	vec3		initial_dir;
	vec3		dir;

	if (set_reset(transform, &initial_rot, ROTATE, et))
	{
		inital_mp = v.mouse_pos;
		return ;
	}
	r = get_rotation_indicator(transform->pos,
			world_to_screenpos(transform->pos));
	draw_modify_axis(r, transform->pos);
	initial_dir = from_to(r.anchor, inital_mp);
	dir = from_to(r.anchor, v.mouse_pos);
	gizmo_line(r.anchor, v.mouse_pos, WHITE);
	transform->rot = plane_alligned_add(initial_rot,
			v_3(atan2(dir.y, dir.x) + MYPI
				- atan2(initial_dir.y, initial_dir.x) + MYPI));
}
// if (v._ctrl) v_round(&transform->rot, 5*DEG2RAD);

void	mrs_null_reference(tfm *_, int et)
{
	printf("ERROR in MRS event :: null reference\n");
}


int	set_reset(tfm *transform, vec3 *init_p, int mode, int et)
{
	if (et == SET)
	{
		if (v.selected == NULL)
			return (1);
		if (mode == MOVE)
			*init_p = transform->pos;
		if (mode == ROTATE)
			*init_p = transform->rot;
		if (mode == SCALE)
			*init_p = transform->scale;
		v.selection_mode = mode;
		v.render_mode = RASTER;
		return (mode_cursor(mode), 1);
	}
	if (et == RESET)
	{
		if (mode == MOVE)
			transform->pos = *init_p;
		if (mode == ROTATE)
			transform->rot = *init_p;
		if (mode == SCALE)
			transform->scale = *init_p;
		return (default_crusor(), 1);
	}
	return (0);
}

// should be on top
void	draw_modify_axis(ri r, vec3 inital_position)
{
	vec3	f;
	vec3	t;

	if (v.selection_mode != NONE && v.plane != XYZ)
	{
		if (X_ENABLED)
		{
			f = v_add(inital_position, v3(v.far));
			t = v_add(inital_position, v3(-v.far));
			draw_projected_line(f, t, X_COLOR);
		}
		if (Y_ENABLED)
		{
			f = v_add(inital_position, v3(0, v.far));
			t = v_add(inital_position, v3(0, -v.far));
			draw_projected_line(f, t, Y_COLOR);
		}
		if (Z_ENABLED)
		{
			f = v_add(inital_position, v3(0, 0, v.far));
			t = v_add(inital_position, v3(0, 0, -v.far));
			draw_projected_line(f, t, Z_COLOR);
		}
	}
}

void	get_intersections(vec3 pos, vec3 mp, vec3 *ir, vec3 *ib)
{
	t_item		temp;
	hit_record	rec;
	ray			raii;

	temp = (t_item){(tfm){pos, v3(), v_3(1)}, new_m(BLACK), NULL, hit_plane};
	set_transform_matrix(&temp.transform, temp.fwd, temp.bck);
	init_ray(mp.x, mp.y, &raii);
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
