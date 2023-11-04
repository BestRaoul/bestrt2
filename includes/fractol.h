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

# define MAX_ITEM_COUNT 100
# define MAX_LIGHT_COUNT 100

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
# define FULL_MAP		BW_MAP(1.0)
# define WHITE_MAP		cc3(WHITE)
# define BLACK_MAP		cc3(BLACK)

# define SPHERE		raster_sphere,	hit_sphere
# define BOX		raster_box,		hit_box
# define CYLINDER	raster_cylinder,hit_cylinder
# define CONE		raster_pyramid,	hit_cone
# define LINE		raster_line,	hit_line
# define PLANE		raster_plane,	hit_plane
# define QUAD		raster_quad,	hit_quad
# define SS_QUAD	raster_quad,	hit_ss_quad

# include "keymap.h"
# include "libft.h"
# include "gc_malloc.h"
# include "mlx_int.h"

# include <math.h>
# include <mlx.h>
# include <stdio.h>
# include <sys/time.h>
# include <time.h>
# include <stdarg.h>
# include <string.h>
# include <X11/cursorfont.h>


//temp
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>

// The Xkb extension provides improved keyboard support
#include <X11/XKBlib.h>

// The XInput extension provides raw mouse motion input
#include <X11/extensions/XInput2.h>
//--temp end

typedef struct s_point		vec3;
typedef struct s_quat		quat;

typedef struct s_ray		ray;
typedef struct s_material	material;
typedef struct s_hit_record	hit_record;
typedef struct s_interval	interval;
typedef struct s_item		t_item;
typedef struct s_texture	texture;

typedef vec3 color;

//row . collumn
typedef double m4x4[4][4];

typedef struct {
	m4x4 mat;
} s_m4;

typedef struct s_quat {
    double x, y, z, w;
}	quat;

typedef struct s_point {
	double	x, y, z;
}	vec3;

typedef struct t_transform {
	vec3	pos;
	vec3	rot;
	vec3	scale;
} tfm;

typedef struct s_ray {
	vec3	orig;
	vec3	dir;
} ray;

typedef struct s_bmp_read {
    int widht;
    int height;
    int **pixels;
}  bmp_read;

typedef struct s_texture {
	color	color_value;
	color	(*value)(double, double, const texture *);
	//checkers
	texture	*checker_0;
	texture	*checker_1;
	double	inv_scale;
	//image
	int		image_width;
	int		image_height;
	color	*image;
	//func
	color	(*uv_func)(vec3);
} texture;

typedef struct s_material {
	//PBR
	texture	base_color;		//(diffuse, base_color)
	texture	metalic;		//(is_metalic, can be texture)  (BW)

	//[subsurface]

	texture	specular;		//(how much, specular probability base on IOR)
	double	specular_tint;	//(lerp between WHITE - base_color)(should always be 0=WHITE)

	texture	roughness;		//(fuzz, ..)  (BW)

	//[anisotropic]
	//[sheen]
	//[clearcoat]

	double	ior;
	texture	transmission;			//(dielectric probability)
	texture	transmission_roughness;	//(BW)

	texture	emission;				//(color)
	double	emission_strength;		//(1.0 = exact same as base_color)
	//texture	emission_strength;  	//(BW)

	texture	normal;
	vec3	normal_strength;
	int		normal_mode;

	texture alpha;

} material;

typedef struct s_hit_record {
	vec3	p;
	vec3	normal;
	vec3	old_normal;
	double	t;
	Bool	front_face;
	material mat;
	double	u;
	double	v;
	t_item	*item;
} hit_record;

typedef struct s_interval {
	double min;
	double max;
} interval;


enum e_plne {
	XYZ,
	XY,
	XZ,
	YZ,
	X,
	Y,
	Z
};

enum e_slct {
	NONE	= 0,
	MOVE	= 1,
	ROTATE	= 2,
	SCALE	= 3
};

enum e_nmode {
	OPENGL,
	DIRECTX
	//idk
};

enum e_render_mode {
	RAYTRACE,
	RAYTRACE_STEPS,
	RAYTRACE_MAT_DEBUG,
	RAYTRACE_UVS,
	RASTER,
	RENDERMODES_MAX,

	//deprecated
	RAYTRACE_STEPS_2,
	RAYTRACE_STEPS_3,
	RAYTRACE_DIST,

	RASTER_HEATMAP,
	RENDER_MOVIE,
};

enum e_mat_debugmode {
	NORMAL,
	MIST,
	
	DIFFUSE_LIGHT,
	DIFFUSE_COLOR,
	SPECULAR_LIGHT,
	SPECULAR_COLOR,

	EMISSION,
	ENVIRONEMENT,
	COMBINED,

	MAT_DEBUGMODES_MAX,
};

typedef struct s__img {
	void	*mlx_img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t__img;

typedef struct s_motion {
	double	*value;
	double	start_val;
	double	end_val;
	double	(*tween)(double, double, double);
} motion;

typedef struct s_shader_end {
	vec3	diffuse_light;
	color	diffuse_color;
	vec3	specular_light;
	color	specular_color;
} shader_end;

typedef struct s_light
{
	tfm		transform;

	vec3	col;
	vec3	dir; //(0, -1, 0) rot3(rot)
	double	intensity; //v_len(scale)
	Bool	is_dir; //whether is a directional light or positional
} t_light;

typedef struct s_item {
	tfm			transform;

	material	mat;
	void 		(*raster)(t_item *);
	Bool		(*hit)(const ray *, const interval, hit_record *, const t_item *);

	m4x4		fwd;
	m4x4		bck;
}	t_item;


typedef struct s_vars {
	int			w;
	int			h;
	void		*mlx;
	void		*win;
	Cursor		cursor;
	t__img		img;
	vec3		**accumulate_img;
	double		**dist_heatmap;

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
	double		delta_time; //time since last update in seconds
 
 //Camera
	vec3		camera_pos;
	vec3		camera_rot;
	quat		camera_quat;
	m4x4 		rotation_matrix;

    double		defocus_angle;
    double		focus_dist;

	int			upscale;
	int			max_depth;

	double		near;
	double		far;

	double		vfov;
	int			lookat_toggle;
	vec3		lookfrom;
	vec3		lookat;
	vec3		vup;
	vec3		pixel00_loc;
	vec3		pixel_delta_u;
	vec3		pixel_delta_v;
	vec3		camera_center;
	vec3		defocus_disk_u;
    vec3		defocus_disk_v;
	Bool		camera_change;
	double		camera_dist_lookat;

	t_item		items[MAX_ITEM_COUNT];
	int			item_count;

	t_light		lights[MAX_LIGHT_COUNT];
	int			light_count;

	int			frame;

	enum e_plne	plane;
	enum e_slct selection_mode;
	tfm			*selected;
	Bool		selection_is_item_lamp;

	motion		*motions;
	int			motion_count;
	Bool		motion_enabled;

	double		time_passed;
	double		time_speed;

	int			max_samples;
	int			samples_per_step;

	double		animation_duration;
	int			animation_framerate;
	double		animation_speed;
	//animation samples
	int			animation_render_mode;
	int			animation_loops;
	Bool		rendering_movie;

	//sun dir
	//sky texture -> skybox

	Bool		cam_flipp;

	Bool		use_background;
	Bool		use_ibl;
	texture		background;
	texture		uv_debug;
	texture		irradiance_map;
	texture		blurry_irradiance_map;

	double		ambient;

	int			mat_debugmode;

	Bool		solo_lighting;

}	t_vars;

extern t_vars	v;

//	ain
void    init_scene(int select);
int		my_exit();
//loop.c
int		loop();
void	render_movie();

// Backgrounds
color	sky_background(vec3 uv);
color	shit_sky_background(vec3 uv);
color	uv_background(vec3 uv);

//disk
//------------skipped

// Draw
// # color
int		new_trgb(int t, int r, int g, int b);
int		new_rgb(int r, int g, int b);
color	new_color(double r, double g, double b);
color	color_lerp(double t, color color1, color color2);
int		color2rgb(color c);
color	rgb2color(int rgb);
int		get_r(int trgb);
int		get_g(int trgb);
int		get_b(int trgb);
// # gizmo
void	gizmo_dot(vec3 pos, color c);
void	gizmo_line(vec3 start, vec3 end, color c);
void	heat_line(vec3 start, vec3 end, color c);
void	gizmo_drag(vec3 start_pos, vec3 current_pos, color c);
void	draw_debug_line(vec3 start, vec3 end, color c);
void	draw_debug_dot(vec3 pos, color c);
void	draw_projected_dot(vec3 pos, color c);
void	draw_projected_point(vec3 pos, color c);
void	draw_projected_line(vec3 start, vec3 end, vec3 c);
// # scribe
void	scribe(char *str, int x, int y, color c);
void	scribe_pos(char *tag, vec3 pos, int x, int y, color c);
void	scribe_v3d(char *tag, vec3 pos, int x, int y, color c);
void	scribe_num(char *format, int n, int x, int y, color c);
void	scribe_dub(char *format, double d, int x, int y, color c);

// Event handlers
int		handle_mouse_move(int x, int y);
int		handle_mouse_press(int button, int x, int y);
int		handle_mouse_release(int button, int x, int y);
int		handle_key_press(int key);
int		handle_key_release(int k);
void	plane_switch(enum e_plne _plane, enum e_plne _excluded);
void	_reset_consumable_clicks(void);

// Image

void	draw_raw(int x, int y, color c);
int		get_pixel(int x, int y);
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
		void (*raster)(t_item *), Bool (*hit)(const ray *, const interval,
			hit_record *, const t_item *));
t_item	*add_item_(t_item t);
void	remove_item(t_item *t_ptr);
t_item	get_item_default(void);
// Lamps
t_light	*add_lamp(color col, vec3 pos_dir, double intensity, Bool is_dir);
t_light	*add_lamp_(t_light l);
void	remove_lamp(t_light *l_ptr);
t_light	get_lamp_default(void);

// Materials
material	default_material(void);
material	new_m(vec3 color);
material	new_m_rgb(vec3 rgbs);
material	new_lambertian(texture t);
material	new_lambertian_bump(texture t, texture bump);
material	new_metal(texture t, double roughness);
material	new_dielectric(texture t, double ior);
// # scatter
void	pbr_scatter(const ray *r, hit_record *e, ray *s, Bool *w);


// ------------------------------------------------------------------------- //

//		-- rotate
vec3	rotate(vec3 v, double o);
vec3	rotate_x(vec3 v, double alpha);
vec3	rotate_y(vec3 v, double beta);
vec3	rotate_z(vec3 v, double gamma);
vec3	rotate3(vec3 v, vec3 r);
vec3	rotate_around(vec3 v, vec3 p, vec3 axis, double angle);



//raster.c
void	raster(void);
//		--Functions
void	raster_square(t_item *item);
void	raster_line(t_item *item);
void	raster_box(t_item *item);
void	raster_pyramid(t_item *item);
void	raster_sphere(t_item *item);
void	raster_cylinder(t_item *item);
void	raster_plane(t_item *item);
void	raster_quad(t_item *item);

//hit
Bool	hit			(const ray *r, const interval ray_t, hit_record *rec);
Bool	hit_sphere	(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
Bool	hit_box		(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
Bool	hit_cylinder(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
Bool	hit_cone	(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
Bool	hit_line	(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
Bool    hit_plane	(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
Bool    hit_quad	(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
Bool	hit_ss_quad	(const ray *r, const interval ray_t, hit_record *rec, const t_item *self);
//triangle
//quad
//cylinder
//cone
Bool	check_hit(const ray *r, const interval ray_t);
Bool	info_hit(const ray *r, const interval ray_t, hit_record *rec);

//raytrace.c
//		--Pixels
void    raytrace(void);
void	init_ray(double x, double y, ray *r);
int		ray_trace(ray *r);

//ui.c
void	help_ui(void);
void	debug_ui(void);



//update
//		--Data
void	update_delta_time(void);
void	apply_motions(void);
void	update_lamp_stats(void);
void	update_tfm_matrices(void);
//		--Item
void	move_transform(tfm *t, Bool set);
void	rotate_transform(tfm *t, Bool set);
void	scale_transform(tfm *t, Bool set);
void	maybe_add_item(void);
void	manage_selection(void);
//		--Camera
void    update_camera(void);


//math.c
double	distance(vec3 a, vec3 b);
int 	sign(int n);
double	signd(double n);
int		min(int a, int b);
int		max(int a, int b);
double	mind(double a, double b);
double	maxd(double a, double b);
int 	in_bounds(int x, int y);
int		v_in_bounds(vec3 pos);
double	fclamp(double i, double max, double min);
//2
vec3    v3_base(vec3 in);
vec3    v_3(double x);
vec3    v_add(vec3 a, vec3 b);
vec3    v_sub(vec3 a, vec3 b);
vec3    v_mult(vec3 a, vec3 b);
vec3    v_div(vec3 a, vec3 b);
double	v_len(vec3 a);
vec3   from_to(vec3 a, vec3 b);
//3
Bool	v_eq(vec3 a, vec3 b);
vec3	v_norm(vec3 a);
vec3	v_scal(vec3 a, double scalar);
double	vec_dist(vec3 a, vec3 b);
double	pt_dist_to_sphere(vec3 pt, vec3 sphere_center, double rad);
vec3	v_cross(vec3 a, vec3 b);
double	v_dot(vec3 a, vec3 b);
double	length_squared(vec3 a);
//4
vec3	line_intersection(vec3 A, vec3 B, vec3 C, vec3 D);
vec3	project_onto_screen_limits(vec3 p, vec3 dir);
//5
void	mm_(const m4x4 a, const m4x4 b, m4x4 c);
s_m4	mm(const m4x4 a, const m4x4 b);
void	matrix_multiplicationconst (m4x4 a, const m4x4 b, m4x4 c);
vec3	mult_point_matrix(const vec3 in, const m4x4 M);
//6
vec3    get_ray_direction(int i, int j);
vec3	ray_at(const ray *r, double t);
double	t_at(const ray *r, vec3 ray);
vec3	lerp(double t, vec3 a, vec3 b);
vec3    local_p(vec3 p, tfm *transform);
//7
double	random_double_l(double min, double max);
double	random_double();
vec3	pixel_sample_square(void);
vec3	defocus_disk_sample(void);
vec3	random_v3();
vec3	random_v3_l(double min, double max);
//a lot of random vector shit
vec3	random_in_unit_sphere();
vec3	random_unit_vector();
vec3	random_on_hemisphere(vec3 normal);

//.
//.
//.
//trashcan.c
void	update_delta_time(void);
vec3	plane_alligned_add(vec3 base, vec3 add);
int		get_elapsed(struct timeval event);
vec3	*get_npoints(int n, double r_offset);
void	print_pos(vec3 pos, char *msg);
int		check_heat(vec3 pos);
void	set_heat(vec3 pos);

vec3	project(vec3 pos, double fov, double aspect);
vec3	reverse_project(vec3 pos, double fov, double aspect);
vec3	world_to_screenpos(vec3 pos);
vec3	world_to_screenpos_fixed_fov(vec3 pos, double fov);
vec3	world_to_screenpos_ortho(vec3 pos);

void	draw_grid_and_cardinals(void);
void	raster_items(void);
void	raster_lamps(void);
void	reset_heatmap(void);

void	set_cursor(unsigned int xc);

quat	euler_to_quaternion(vec3 euler);
vec3	quaternion_to_euler(quat q);
vec3	rotate3d(vec3 vector, quat quaternion);
vec3	rotate_ly(vec3 position, vec3 rotation);

double	linear_to_gamma(double linear_component);

Bool	near_zero(vec3 e);
Bool	close_enough(double __x);
vec3	reflect(vec3 v, vec3 n);
vec3	reflect_safe(vec3 v, vec3 n, vec3 old_n);
vec3	refract(vec3 uv, vec3 n, double etai_over_etat);
double	reflectance(double cosine, double ref_idx);

vec3	lookRotation(vec3 lookfrom, vec3 lookat);
vec3	look_at(vec3 lookfrom, vec3 lookat, vec3 up);

double	smoothstep (double edge0, double edge1, double x);
double	realistic_specular(double ior);

vec3	parent_to(vec3 v, const tfm *transform);

double	ACES(double x);

// ------Interval
Bool	contains(interval _t, double x);
Bool	surrounds(interval _t, double x);
double	clamp(interval _t, double x);
double	clamp_(double x);

// ------Textures
//deepcopy
texture		*t_shallow_copy(texture *t);
texture		*t_deep_copy(texture *t);
texture		solid_color(color c);
texture		checkerboard(double scale, texture even, texture odd);
texture		from_bmp(const char *filename);
texture		from_func(color (*uv_func)(vec3));
color  		evaluate(texture *t, double u, double v);
double   	evaluate_bw(texture *t, double u, double v);
color   	evaluate_spread(texture *t, double _u, double _v, double _spread);
color		getGaussianBlur(int x, int y, const texture *self);

// ------Tweens
void	add_motion(double *value, double start_value, double end_value, double (*tween)(double, double , double));
double	lerpd(double a, double b, double t);
double	ping_pong(double a, double b, double t);
double	ping_pong_2(double a, double b, double t);
double	sin_tween(double a, double b, double t);
double	cos_tween(double a, double b, double t);
double	easeInOutCubic(double a, double b, double t);

// ------Normals
vec3    perturb_normal(vec3 normal, vec3 perturbation);
vec3    texture_diff_bw(texture *t, vec3 uv);
vec3    compute_rgb_perturbation(texture *t, vec3 normal, vec3 uv);

void	maybe_apply_perturb(hit_record *rec);

// ------Output to disk
void write_img(void);
void ffmpeg_bmp_to_mp4(int framerate, int loops);
int	readBMP(const char* filename, bmp_read *r);

// ------PBR shenenigans
shader_end	CalcTotalPBRLighting(hit_record *rec, ray *ray_in);

// -----Transformation Mx4
void set_transform_matrix(const tfm *t, m4x4 m, m4x4 m_bck);
void create_transform_matrix(const tfm* t, m4x4 result);
void multiply_matrix_vector(const m4x4 mat, const vec3 in, vec3* out);
void multiply_matrix_vector_2(const m4x4 M, const vec3 in, vec3* out);
void print_mx4(m4x4 matrix);
ray	 apply_ray(const ray *in, const m4x4 t);

//TRASHHHHH
void    simulate_rayzz();
typedef struct {
	vec3	anchor;
	vec3	r_top, r_bot;
	vec3	g_top, g_bot;
	vec3	b_top, b_bot;
} ri;
ri	get_rotation_indicator(vec3 anchor, vec3 end);

void	fswap(double *__x, double *__y);
void	vswap(vec3 *__x, vec3 *__y);
void	**alloc_2d(int size, int w, int h);


# define INTERVAL_EMPTY (interval){+INFINITY, -INFINITY}
# define INTERVAL_UNIVERSE (interval){-INFINITY, +INFINITY}
# define INTERVAL_FORWARD (interval){0, +INFINITY}

#endif
