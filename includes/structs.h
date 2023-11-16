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

#ifndef STRUCTS_H
# define STRUCTS_H

#include <X11/Xlib.h>

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
typedef PFPN m4x4[4][4];

typedef struct {
	m4x4 mat;
} s_m4;

typedef struct s_quat {
    PFPN x, y, z, w;
}	quat;

typedef struct s_point {
	PFPN	x, y, z;
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
	color	(*value)(PFPN, PFPN, const texture *);
	//checkers
	texture	*checker_0;
	texture	*checker_1;
	PFPN	inv_scale;
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
	PFPN	specular_tint;	//(lerp between WHITE - base_color)(should always be 0=WHITE)

	texture	roughness;		//(fuzz, ..)  (BW)

	//[anisotropic]
	//[sheen]
	//[clearcoat]

	PFPN	ior;
	texture	transmission;			//(dielectric probability)
	texture	transmission_roughness;	//(BW)

	texture	emission;				//(color)
	PFPN	emission_strength;		//(1.0 = exact same as base_color)
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
	PFPN	t;
	Bool	front_face;
	material mat;
	PFPN	u;
	PFPN	v;
	t_item	*item;
} hit_record;

typedef struct s_interval {
	PFPN min;
	PFPN max;
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
	OPENGL = 0,
	DIRECTX = 1
	//idk
};

enum e_render_mode {
	RAYTRACE,
	RAYTRACE_STEPS,
	RAYTRACE_MAT_DEBUG,
	RAYTRACE_MAT_VALUES,
	RAYTRACE_UVS,
	RASTER,
	RENDERMODES_MAX,

	//deprecated
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

enum e_mat_valuesmode {
	V_ALBEDO,
	V_METALIC,
	V_SPECULAR,
	V_ROUGHNESS,
	V_TRANSMISSION,
	V_TRANSMISSION_ROUGHNESS,
	V_EMISSION,
	V_NORMAL,
	V_ALPHA,

	MAT_VALUESMODES_MAX,
};

enum e_tone_mode {
	RAW,
	GAMMA,
	ACES,
	HDR,
	ACES_GAMMA,
	HDR_GAMMA,
	ACES_HDR_GAMMA,
	HDR_ACES_GAMMA,
	TONEMODES_MAX,
};

enum e_skip_mode {
	NOSKIP,
	HALFSKIP,
	ONETWOTHSKIP,
	SKIPMODES_MAX
};

//skip mode

typedef struct s__img {
	void	*mlx_img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t__img;

typedef struct s_motion {
	PFPN	*value;
	PFPN	start_val;
	PFPN	end_val;
	PFPN	(*tween)(PFPN, PFPN, PFPN);
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
	PFPN	intensity; //v_len(scale)
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

typedef struct s_split_task {
	int		x_off;
	int		y_off;
	int		w_size;
	int		h_size;
	int		upscale;
	Bool	noblock;
	int		_x;
	int		_y;
}	t_split_task;

typedef struct {
	vec3	anchor;
	vec3	r_top, r_bot;
	vec3	g_top, g_bot;
	vec3	b_top, b_bot;
} ri;

enum e_et {
	SET = 0,
	UPDATE,
	CONFIRM,
	RESET
};

static inline void	nohit(PFPN *t, PFPN *u, PFPN *v)
{
	*t = 100e6;
	*u = 0.0;
	*v = 0.0;
}

static inline void	set(PFPN *x, PFPN _x)
{
	*x = _x;
}

static inline void	seti(int *x, int _x)
{
	*x = _x;
}

typedef struct
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
}   unit8_color;

typedef struct s_bmp_header
{
	uint16_t		signature;
	uint32_t		file_size;
	uint16_t		reserved1;
	uint16_t		reserved2;
	uint32_t		data_offset;
	uint32_t		header_size;

	int32_t			width;
	int32_t			height;
	uint16_t		planes;
	uint16_t		bpp;
}					t_bmp_header;

#endif
