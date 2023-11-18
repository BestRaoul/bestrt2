/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# define ANSI_RESET   "\x1b[0m"
# define ANSI_RED     "\x1b[31m"
# define ANSI_GREEN   "\x1b[32m"
# define ANSI_YELLOW  "\x1b[33m"
# define ANSI_BLUE    "\x1b[34m"
# define ANSI_BOLD	  "\x1b[1m"

# define PFPN	double

# define ILLEGAL
# include <stdio.h>
# include <stdbool.h>

# define True 1
# define False 0

# define WARNING "["ANSI_BOLD""ANSI_YELLOW"WARNING"ANSI_RESET"]"
# define ERROR	 "["ANSI_BOLD""ANSI_RED"ERROR"ANSI_RESET"]"

static inline void	notify_illegal(const char *feature)
{
	dprintf(2, WARNING" \
You are trying to invoke \"%s\", an ILLEGAL feature.\n", feature);
	dprintf(2, "Please recompile with that flag\n");
}

# define FF	"\%03d.bmp"
# define OUTFOLDER "output"

# define MAX_ITEM_COUNT 100
# define MAX_LIGHT_COUNT 100

# define TARGET_FPS 60

# define MYPI 3.1415
# define DEG2RAD MYPI/180.0
# define RAD2DEG 180.0/MYPI

# define v3(...) (vec3){__VA_ARGS__}
# define vrgb(r,g,b) (color){r/256.0, g/256.0, b/256.0}
# define c3(...) solid_color((color){__VA_ARGS__})
# define cc3(x)	solid_color(x)

# define CENTER v3(v.w/2, v.h/2)
# define CORNER v3(v.w, v.h)

# define NOT_IMPLEMENTED(f) printf("%s feature is not implemented yet..\n", f);

# define X_COLOR vrgb(223,63,80)
# define Y_COLOR vrgb(128,187,27)
# define Z_COLOR vrgb(65,131,196)

# define X_ENABLED (v.plane != YZ && v.plane != Y && v.plane != Z)
# define Y_ENABLED (v.plane != XZ && v.plane != X && v.plane != Z)
# define Z_ENABLED (v.plane != XY && v.plane != X && v.plane != Y)

# define RED	new_color(1, 0, 0)
# define GREEN	new_color(0, 1, 0)
# define BLUE	new_color(0, 0, 1)
# define WHITE	new_color(1, 1, 1)
# define BLACK	new_color(0, 0, 0)

# define ERROR_CYAN new_color(0, 1, 1)

# define BW_MAP(w)		c3(w,w,w)
# define NO_MAP			BW_MAP(0.0)
# define HALF_MAP		BW_MAP(0.5)
# define FULL_MAP		BW_MAP(1.0)
# define WHITE_MAP		cc3(WHITE)
# define BLACK_MAP		cc3(BLACK)

# define SPHERE		raster_sphere,	hit_sphere
# define BOX		raster_box,		hit_box
# define CYLINDER	raster_cylinder,hit_cylinder
# define CONE		raster_pyramid,	hit_cone
# define PLANE		raster_plane,	hit_plane
# define QUAD		raster_quad,	hit_quad
# define SS_QUAD	raster_quad,	hit_ss_quad

# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdint.h>

# include "keymap.h"
# include "structs.h"
# include "libft.h"
# include "gc_malloc.h"
# include "gnl.h"

# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <mlx.h>
# include <stdio.h>
# include <sys/time.h>
# include <time.h>
# include <stdarg.h>
# include <string.h>

# ifdef LINUX
#  include "mlx_int.h"
#  include <X11/cursorfont.h>
# endif
//temp
// #include <X11/Xlib.h>
// #include <X11/keysym.h>
// #include <X11/Xatom.h>
// #include <X11/Xresource.h>

// The Xkb extension provides improved keyboard support
// #include <X11/XKBlib.h>

// The XInput extension provides raw mouse motion input
// #include <X11/extensions/XInput2.h>
//--temp end

typedef struct s_vars {
	int			w;
	int			h;
	void		*mlx;
	void		*win;
# ifdef LINUX
	Cursor		cursor;
# endif
	t__img		img;
	vec3		**accumulate_img;
	PFPN		**dist_heatmap;

	//mouse
	int			_lmouse;
	int			_rmouse;
	int			_scroll;
	vec3		lm_start_pos;
	vec3		rm_start_pos;
	vec3		mouse_pos;
	//^toggles
	int			_debug;
	int			_help;
	//holds
	int			_up;
	int			_down;
	int			_left;
	int			_right;
	int			_q;
	int			_e;

	int			_space;
	int			_shift;
	int			_ctrl;
	//events
	int			_0;
	int			_1;
	int			_2;
	int			_3;
	int			_4;
	int			_5;
	int			_6;
	int			_7;
	int			_8;
	int			_9;

	int			_np0;
	int			_np1;
	int			_np2;
	int			_np3;
	int			_np4;
	int			_np5;
	int			_np6;
	int			_np7;
	int			_np8;
	int			_np9;

	int			_lclick;
	int			_rclick;
	int			_llift;
	int			_rlift;
	int			_p;
	int			_rerender;

	//--end--//
	int			orthographic_toggle;
	int			render_mode;

	//data
	struct timeval	last_update;
	PFPN		delta_time; //time since last update in seconds
 
 //Camera
	vec3		camera_pos;
	vec3		camera_rot;
	quat		camera_quat;
	m4x4 		rotation_matrix;

    PFPN		defocus_angle;
    PFPN		focus_dist;

	int			upscale;
	int			max_depth;

	PFPN		near;
	PFPN		far;

	PFPN		vfov;
	int			lookat_toggle;
	vec3		lookat;
	vec3		vup;
	vec3		pixel00_loc;
	vec3		pixel_delta_u;
	vec3		pixel_delta_v;
	vec3		camera_center;
	vec3		defocus_disk_u;
    vec3		defocus_disk_v;
	bool		camera_change;
	PFPN		camera_dist_lookat;
	vec3		camera_dir;

	t_item		items[MAX_ITEM_COUNT];
	int			item_count;

	t_light		lights[MAX_LIGHT_COUNT];
	int			light_count;

	int			frame;

	enum e_plne	plane;
	enum e_slct selection_mode;
	tfm			*selected;
	bool		selection_is_item_lamp;

	motion		*motions;
	int			motion_count;
	bool		motion_enabled;

	PFPN		time_passed;
	PFPN		time_speed;

	int 		samples;
	int			steps_rendered;

	int			max_samples;
	int			samples_per_step;

	PFPN		animation_duration;
	int			animation_framerate;
	PFPN		animation_speed;
	//animation samples
	int			animation_render_mode;
	int			animation_loops;
	bool		rendering_movie;

	//sun dir
	//sky texture -> skybox

	bool		cam_flipp;

	bool		use_background;
	bool		use_ibl;
	texture		background;
	texture		uv_debug;
	texture		irradiance_map;
	texture		blurry_irradiance_map;

	PFPN		ambient;
	color		ambient_color;

	int			mat_debugmode;
	int			mat_valuesmode;
	int			tone_mode;
	int			skip_mode;

	bool		solo_lighting;

	bool		is_headless_print;

}	t_vars;

extern t_vars	v;

// ------------------------------------------------------------------------- //

// Main
void    init_scene(int select);
int		my_exit();

// Loop
int		loop();
void	render_movie();
void	headless_print(void);

// Backgrounds
color	sky_background(vec3 uv);
color	shit_sky_background(vec3 uv);
color	uv_background(vec3 uv);
color	stars(vec3 uv);

// Disk
// # ffmpeg
void	ffmpeg_bmp_to_mp4(int framerate, int loops);
// # read
bool	read_bmp(const char *filename, bmp_read *r);
bool	read_xmp(const char *filename, bmp_read *r);
bool	read_png(const char *filename, bmp_read *r);
bool	read_header(FILE *file, t_bmp_header *header, const char *filename);
bool	read_palette(FILE *file, t_bmp_header *header, int palette[256]);
void	init_pixel_read(t_bmp_header *header, bmp_read *r);
void	pixel_read(FILE *file, t_bmp_header *h, int p[256], bmp_read *r);
// # write
void	write_bmp(const char *filename, int w, int h, unit8_color *ps);
void	write_img(void);

// Draw
// # color
int		new_trgb(int t, int r, int g, int b);
int		new_rgb(int r, int g, int b);
color	new_color(PFPN r, PFPN g, PFPN b);
color	color_lerp(PFPN t, color color1, color color2);
int		color2rgb(color c);
color	rgb2color(int rgb);
int		get_r(int trgb);
int		get_g(int trgb);
int		get_b(int trgb);
// # gizmo
void	draw_line(vec3 start, vec3 end, color c, int (*draw_func)(vec3 p, color c));
void	gizmo_dot(vec3 pos, color c);
void	gizmo_line(vec3 start, vec3 end, color c);
void	heat_line(vec3 start, vec3 end, color c);
void	gizmo_drag(vec3 start_pos, vec3 current_pos, color c);
void	draw_debug_line(vec3 start, vec3 end, color c);
void	draw_debug_dot(vec3 pos, color c);
void	draw_projected_dot(vec3 pos, color c);
// void	draw_projected_point(vec3 pos, color c);
void	draw_projected_line(vec3 start, vec3 end, vec3 c);
// # scribe
void	scribe(char *str, vec3 at, color c);
void	scribe_pos(char *tag, vec3 pos, vec3 at, color c);
void	scribe_v3d(char *tag, vec3 pos, vec3 at, color c);
void	scribe_num(char *format, int n, vec3 at, color c);
void	scribe_dub(char *format, PFPN d, vec3 at, color c);

// Event handlers
int		handle_mouse_move(int x, int y);
int		handle_mouse_press(int button, int x, int y);
int		handle_mouse_release(int button, int x, int y);
int		handle_key_press(int key);
void	handle_submode_next(void);
int		handle_key_release(int k);
void	plane_switch(enum e_plne _plane, enum e_plne _excluded);
void	_reset_consumable_clicks(void);

// Hit functions
bool	hit(const ray *r, const interval ray_t, hit_record *rec);
bool	info_hit(const ray *r, const interval ray_t, hit_record *rec);
bool	check_hit(const ray *r, const interval ray_t);
// # 2 helpers
void	set_face_normal(hit_record *rec, const ray *r, const vec3 on);
PFPN	t2global(const PFPN lt, const ray *lr, const ray *r, const m4x4 fwd);
bool	is_interior(const PFPN a, const PFPN b);
vec3	get_cube_normal(const int id, hit_record *rec);
// # HITS
bool	hit_sphere(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
bool    hit_plane(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
bool    hit_quad(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
bool    hit_ss_quad(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
bool    hit_box(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
bool    hit_cylinder(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
bool    hit_cone(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);

// # ehlps
void	save_box_intersection(PFPN t[2], PFPN u[2], PFPN v[2], const ray *local_r);

// Image
void	draw_raw(int x, int y, color c);
int		get_pixel(int x, int y);
int		get_pixel_(int x, int y, t__img *image);
int		draw_inbounds(int x, int y, color c);
int		draw_v_inbounds(vec3 p, color c);
int		draw_v_inheat(vec3 p, color c);
void	clear_img(t__img img);
color	hdr_tone(color c);
color	aces_tone(color c);
color	gamma_correct(color c);
void	draw_gamma_corrected(int x, int y, color c);


// Items
t_item	*add_item(vec3 p, vec3 s, vec3 r, material m,
			void (*raster)(t_item *), bool (*hit)(const ray *, const interval,
				hit_record *, const t_item *));
t_item	*add_item_(t_item t);
void	remove_item(t_item *t_ptr);
t_item	get_item_default(void);
// Lamps
t_light	*add_lamp(color col, vec3 pos_dir, PFPN intensity, bool is_dir);
t_light	*add_lamp_(t_light l);
void	remove_lamp(t_light *l_ptr);
t_light	get_lamp_default(void);

// Materials
material	default_material(void);
material	new_m(vec3 color);
material	new_m_rgb(vec3 rgbs);
material	new_lambertian(texture t);
material	new_lambertian_bump(texture t, texture bump);
material	new_metal(texture t, PFPN roughness);
material	new_dielectric(texture t, PFPN ior);
material	new_light(texture t, PFPN emission_strength);
// # scatter
void	pbr_scatter(const ray *r, hit_record *e, ray *s, bool *w);

// Math
int		max(int a, int b);
int		min(int a, int b);
PFPN	mind(PFPN a, PFPN b);
PFPN	maxd(PFPN a, PFPN b);
PFPN	fclamp(PFPN x, PFPN min, PFPN max);
int		sign(int n);
PFPN	signd(PFPN n);
vec3	lerp(PFPN t, vec3 a, vec3 b);
vec3	rotate_x(vec3 v, PFPN alpha);
vec3	rotate_y(vec3 v, PFPN beta);
vec3	rotate_z(vec3 v, PFPN gamma);
vec3	rotate3(vec3 v, vec3 r);
vec3	rotate_around(vec3 v, vec3 p, vec3 axis, PFPN angle);
bool	contains(interval _t, PFPN x);
bool	surrounds(interval _t, PFPN x);
PFPN	clamp(interval _t, PFPN x);
PFPN	clamp_(PFPN x);
PFPN	aces(PFPN x);
PFPN	linear_to_gamma(PFPN linear_component);
bool	near_zero(vec3 e);
bool	close_enough(PFPN __x);
// # matrix
s_m4	mm(const m4x4 a, const m4x4 b);
void	multiply_matrix_vector(const m4x4 mat, const vec3 in, vec3 *out);
vec3	mult_point_matrix(const vec3 in, const m4x4 M);
void	print_mx4(m4x4 m);
void	set_m4(m4x4 m, const m4x4 s);
void	set_identity(m4x4 m);
void	inverse(const m4x4 in, m4x4 out);
// # projection
vec3	world_to_local(vec3 cam_pos, vec3 pos);
vec3	reverse_project(vec3 pos, PFPN vfov, PFPN aspect);
vec3	project(vec3 pos, PFPN vfov, PFPN aspect);
vec3	project_ortho(vec3 pos, PFPN vfov, PFPN aspect);
vec3	clip_space_to_viewport(vec3 pos);
vec3	world_to_viewport(vec3 cam_pos, PFPN vfov, PFPN aspect, vec3 pos);
vec3	world_to_screenpos(vec3 pos);
vec3	world_to_screenpos_fixed_fov(vec3 pos, PFPN fov);
vec3	world_to_screenpos_ortho(vec3 pos);
// # quaternion
quat	euler_to_quaternion(vec3 euler);
vec3	quaternion_to_euler(quat q);
void	quaternion_to_m4x4(quat q, m4x4 m);
quat	quaternion_multiply(quat a, quat b);
quat	angle_axis(PFPN angle, vec3 axis);
// # random
PFPN	random_pfpn(void);
PFPN	random_normal_distribution(void);
PFPN	random_pfpn_l(PFPN min, PFPN max);
vec3	pixel_sample_square(void);
vec3	random_in_unit_disk(void);
vec3	defocus_disk_sample(void);
vec3	random_v3(void);
vec3	random_v3_l(PFPN min, PFPN max);
vec3	random_in_unit_sphere(void);
vec3	random_unit_vector(void);
// # ray
vec3	get_ray_direction(int i, int j);
vec3	ray_at(const ray *r, PFPN t);
vec3	local_p(vec3 p, tfm *transform);
ray		apply_ray(const ray *in, const m4x4 t);
// # reflection
vec3	reflect(vec3 v, vec3 n);
vec3	reflect_safe(vec3 v, vec3 n, vec3 old_n);
vec3	refract(vec3 uv, vec3 n, PFPN etai_over_etat);
PFPN	reflectance(PFPN cosine, PFPN ref_idx);
PFPN	realistic_specular(PFPN ior);
// # trash
vec3	plane_alligned_add(vec3 base, vec3 add);
vec3	*get_npoints(int n, PFPN r_offset);
void	set_npoints(vec3 *points, int n, PFPN r_offset);
vec3	look_rotation(vec3 lookfrom, vec3 lookat);
vec3	look_at(vec3 lookfrom, vec3 lookat, vec3 up);
// # tweens
void	add_motion(PFPN *value, PFPN start_value, PFPN end_value,
			PFPN (*tween)(PFPN, PFPN, PFPN));
PFPN	lerpd(PFPN a, PFPN b, PFPN t);
PFPN	ping_pong(PFPN a, PFPN b, PFPN t);
PFPN	ping_pong_2(PFPN a, PFPN b, PFPN t);
PFPN	sin_tween(PFPN a, PFPN b, PFPN t);
PFPN	cos_tween(PFPN a, PFPN b, PFPN t);
PFPN	ease_in_out_cubic(PFPN a, PFPN b, PFPN t);
PFPN	smoothstep(PFPN edge0, PFPN edge1, PFPN x);
// # vector
vec3	v_add(vec3 a, vec3 b);
vec3	from_to(vec3 a, vec3 b);
vec3	v_sub(vec3 a, vec3 b);
vec3	v_mult(vec3 a, vec3 b);
vec3	v_div(vec3 a, vec3 b);
vec3	v3_base(vec3 in);
vec3	v_3(PFPN x);
bool	v_eq(vec3 a, vec3 b);
vec3	v_norm(vec3 a);
vec3	swap_yz(vec3 v);
int		in_bounds(int x, int y);
int		v_in_bounds(vec3 pos);
PFPN	length_squared(vec3 a);
PFPN	v_len(vec3 a);
PFPN	vec_dist(vec3 a, vec3 b);
vec3	v_scal(vec3 a, PFPN scalar);
PFPN	v_dot(vec3 a, vec3 b);
vec3	v_cross(vec3 a, vec3 b);
vec3	line_intersection(vec3 A, vec3 B, vec3 C, vec3 D);
vec3	project_onto_screen_limits(vec3 p, vec3 dir);
vec3	parent_to(vec3 v, const tfm *transform);
vec3	parent_to_virtual(vec3 v, vec3 pos, vec3 rot, vec3 scale);
PFPN	distanceToLine(vec3 A, vec3 B, vec3 C);
vec3	project_onto_line(vec3 A, vec3 B, vec3 C);
PFPN	directioned_distance(vec3 from, vec3 to, vec3 dir);
PFPN	tddd(vec3 from, vec3 to, vec3 dir);
PFPN	get_angle(vec3 dir);
void	v_round(vec3 *in, PFPN precision);

// Parsing
int		parse_file(const char *filename);
void	parse_2nd_argument(int ac, char **av);
// # atoos
vec3	atov3(const char *str, vec3 __default);
PFPN	atod(const char *str, PFPN __default);
void	populate_defaults(void);
texture	atotexture(const char *str, texture __default);
material	atomaterial(const char **line, material __default);
bool	get_id(const char *tag, const char **dictionary, int *id);
// #2
int	parse_ibl(const char **line);
int	parse_bg(const char **line);
int	parse_ambient_light(const char **line);
int	parse_point_lamp(const char **line);
int	parse_directional_lamp(const char **line);
// #3 camera
vec3	dir_to_rot(vec3 dir);
int	parse_camera(const char **line);
int	parse_camera_extension(const char **line);
int	parse_render_settings(const char **line);
int	parse_animation_settings(const char **line);
// # line parsing
int	parse_sphere(const char **line);
int	parse_plane(const char **line);
int	parse_quad(const char **line);
int	parse_ss_quad(const char **line);
int	parse_box(const char **line);
int	parse_cylinder(const char **line);
int	parse_cone(const char **line);
int	parse_texture(const char **line);
int	parse_material(const char **line);
void	print_rt_format(void);


// PBR lighting
shader_end	pbr_lighting(hit_record *rec, ray *ray_in);
PFPN	distribution_ggx(PFPN nDotH, PFPN roughness);
PFPN	geometry_smith(PFPN NdotV, PFPN NdotL, PFPN roughness);
vec3	fresnel_shlick(PFPN HdotV, PFPN R0);
bool	check_obstruction(t_light *l, vec3 p);

// Raster
void	raster(void);
void	raster_items(void);
void	raster_lamps(void);
void	raster_selection(void);
// # helpers
void	draw_grid_and_cardinals(void);
void	draw_light_gizmo(t_light *l);
void	__swap_yz(vec3 *v);
// void	draw_lookat(void);
// # functions
void	raster_sphere(t_item *item);
void	raster_cylinder(t_item *item);
void	raster_box(t_item *item);
void	raster_pyramid(t_item *item);
void	raster_quad(t_item *item);
void	raster_plane(t_item *item);
void	link_lines(vec3 *points, int count, color c);
void	dot_them(vec3 *points, int count, color c);
// # heatmap
void	reset_heatmap(void);
bool	check_heat(vec3 pos);
void	set_heat(vec3 pos);

// Raytrace
// # paint
color	paint_dist(bool did_hit, PFPN d);
color	paint_uv(bool did_hit, hit_record *rec);
color	paint_env(vec3 r_dir);
color	mat_values(bool did_hit, hit_record *rec);
color	paint_se(shader_end *se);
bool	is_unlit(int mode);
color	paint_mat_debug_unlit(bool did_hit, hit_record *rec, ray *r);
color	paint_mat_debug_lit(ray *ray_in, hit_record *rec);
// # pixel
void	init_ray(PFPN x, PFPN y, ray *r);
color	trace(ray *r, int max_depth);
void    render_pixel(int x, int y);
// # tasks
void	generate_tasks(t_split_task *tasks, int w_splits, int h_splits, int upscale);
bool	fullfill(t_split_task *task, int _split);
void	show_progress(int _split, int max_splits, struct timeval frame_start);
void	print_progress(PFPN  progress);
// # trace
color	tone(color light);
color	trace(ray *r, int max_depth);
// # render
void	init_ray(PFPN x, PFPN y, ray *r);
color	trace(ray *r, int max_depth);
void    render_pixel(int x, int y);
void	raytrace(void);

// Scenes
void    init_scene(int select);
void    default_cam(void);
void    cornell(vec3 dimensions);
void	test(void);
// # showoff
void	showoff_1(void);
void	showoff_2(void);
void	showoff_3(void);
void	showoff_4(void);
void	showoff_5(void);
void	showoff_6(void);
void	showoff_7(void);
void	showoff_8(void);
void	showoff_9(void);
// # 4-7
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

// Simulation
void	simulate_rayzz(void);

// Textures
texture	solid_color(color c);
texture	checkerboard(PFPN scale, texture even, texture odd);
texture	from_bmp(const char *filename);
texture	from_xmp(const char *filename);
texture	from_png(const char *filename);
texture	from_func(color (*uv_func)(vec3));
texture	*t_shallow_copy(texture *t);
texture	*t_deep_copy(texture *t);
color	evaluate(texture *t, PFPN _u, PFPN _v);
PFPN	evaluate_bw(texture *t, PFPN _u, PFPN _v);
color	evaluate_spread(texture *t, PFPN _u, PFPN _v, PFPN _spread);
// # returns
color   solid_color_return(PFPN _u, PFPN _v, const texture *self);
color   checkeboard_return(PFPN _u, PFPN _v, const texture *self);
color   image_return(PFPN _u, PFPN _v, const texture *self);
color   func_return(PFPN _u, PFPN _v, const texture *self);

// Transform
void    set_transform_matrix(const tfm *transform, m4x4 m_fwd, m4x4 m_bck);

// UI
void	debug_ui(void);
void	help_ui(void);
ri		get_rotation_indicator(vec3 anchor, vec3 end);

// Update camera
void	update_camera(void);
void	maybe_flipp(void);
PFPN	get_move_speed(void);
PFPN	get_angular_speed(void);
void	do_orbit_move(vec3 axis, PFPN angle);
void	rotate_camera(void);
void	move_camera(void);
void	fov_camera(void);

// Update data
int		get_elapsed(struct timeval event);
void	update_delta_time(void);
void	apply_motions(void);
void	update_lamp_stats(void);
void	update_tfm_matrices(void);

// Update item
void	manage_selection(void);
void	maybe_add_item(void);
// # 2 - 3
void	move_transform(tfm *transform, int et);
void	scale_transform(tfm *transform, int et);
void	rotate_transform(tfm *transform, int et);
void	mrs_null_reference(tfm *_, int et);
int		set_reset(tfm *t, vec3 *ip, int mode, int et);
void	draw_modify_axis(ri r, vec3 inital_position);
void	get_intersections(vec3 pos, vec3 mp, vec3 *ir, vec3 *ib);

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

// TRASH
// # 1
void	maybe_apply_perturb(hit_record *rec);
vec3	perturb_normal(vec3 normal, vec3 perturbation);
void	print_pos(vec3 pos, char *msg);
void	fswap(PFPN *__x, PFPN *__y);
void	vswap(vec3 *__x, vec3 *__y);
// # +
bool	check_heat(vec3 pos);
void	set_heat(vec3 pos);
void	**alloc_2d(int size, int w, int h);
void	set_cursor(unsigned int xc);

color	invert_color(color c);
color	get_inverted(vec3 p);

color	gaussian_blur(int x, int y, const texture *self);

void	default_crusor(void);
void	mode_cursor(int mode);

char	**_gc_add_2d(char **ptr_2d);

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

# define INTERVAL_EMPTY (interval){+INFINITY, -INFINITY}
# define INTERVAL_UNIVERSE (interval){-INFINITY, +INFINITY}
# define INTERVAL_FORWARD (interval){0, +INFINITY}

#endif
