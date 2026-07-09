
#include "fractol.h"

void	maybe_apply_perturb(hit_record *rec)
{
	vec3	normal_rgb;
	vec3	perturbation;
	vec3	new_normal;

	normal_rgb = evaluate(&rec->mat.normal, rec->u, rec->v);
	if (v_eq(normal_rgb, v3(0.5, 0.5, 1.0)))
	{
		rec->old_normal = rec->normal;
		return ;
	}
	perturbation = v3(normal_rgb.x * 2 - 1, normal_rgb.y * 2 - 1, normal_rgb.z);
	perturbation = v_mult(perturbation, rec->mat.normal_strength);
	perturbation.z = fabs(perturbation.z);
	new_normal = perturb_normal(rec->normal, perturbation);
	rec->old_normal = rec->normal;
	rec->normal = new_normal;
}

vec3	perturb_normal(vec3 normal, vec3 perturbation)
{
	vec3	new_up;
	vec3	p_v;
	vec3	p_u;
	vec3	output;

	new_up = v3(0, 0, -1);
	if (normal.z > 0.99 || normal.z < -0.99)
		new_up = v3(1, 0, 0);
	p_v = v_cross(new_up, normal);
	p_v = v_norm(p_v);
	p_u = v_cross(normal, p_v);
	p_u = v_norm(p_u);
	output = normal;
	output = v_add(output, v_scal(p_u, perturbation.y));
	output = v_add(output, v_scal(p_v, -perturbation.x));
	output = v_add(output, v_scal(normal, perturbation.z));
	output = v_norm(output);
	return (output);
}

void	print_pos(vec3 pos, char *msg)
{
	printf("%s (%.4f, %.4f, %.4f)\n", msg, pos.x, pos.y, pos.z);
}

void	fswap(PFPN *__x, PFPN *__y)
{
	PFPN	__temp;

	__temp = *__x;
	*__x = *__y;
	*__y = __temp;
}

void	vswap(vec3 *__x, vec3 *__y)
{
	vec3	__temp;

	__temp = *__x;
	*__x = *__y;
	*__y = __temp;
}


void	**alloc_2d(int size, int w, int h)
{
	void	*mem_start;
	void	**ptr_ptr;
	int		i;

	mem_start = gc_malloc(size * w * h);
	ptr_ptr = gc_malloc(sizeof(void *) * w);
	i = 0;
	while (i < w)
	{
		ptr_ptr[i] = (mem_start + i * h * size);
		i++;
	}
	return (ptr_ptr);
}

#ifdef LINUX

void	set_cursor(unsigned int xc)
{
	t_xvar		*xvar;
	t_win_list	*win;
	Cursor		cursor;

	xvar = (t_xvar *)v.mlx;
	win = (t_win_list *)v.win;
	xc %= 154;
	if (v._debug)
		printf("xc: %u\n", xc);
	cursor = XCreateFontCursor(xvar->display, xc);
	XDefineCursor(xvar->display, win->window, cursor);
	XMoveWindow(xvar->display, win->window, 955, 0);
}
#else

void	set_cursor(unsigned int xc)
{
	static int	notified = 0;

	if (!notified)
		dprintf(2, WARNING" Cursor modification disabled as not on LINUX\n");
	notified = 1;
	(void) xc;
}
#endif

color	invert_color(color c)
{
	return ((color){1. - c.x, 1. - c.y, 1. - c.z});
}

color	get_inverted(vec3 p)
{
	return (invert_color(rgb2color(get_pixel(p.x, p.y))));
}


//XC_crosshair
#define CUR_CROSSHAIR 34
//XC_fleur
#define CUR_FLEUR 52
//XC_exchange
#define CUR_EXCHANGE 50
//XC_sizing
#define CUR_SIZING 120

void	default_crusor(void)
{
	set_cursor(CUR_CROSSHAIR);
}

void	mode_cursor(int mode)
{
	if (mode == MOVE)
		set_cursor(CUR_FLEUR);
	if (mode == ROTATE)
		set_cursor(CUR_EXCHANGE);
	if (mode == SCALE)
		set_cursor(CUR_SIZING);
}

PFPN	g_kernel[5][5] = {
{1.0, 4.0, 6.0, 4.0, 1.0},
{4.0, 16.0, 24.0, 16.0, 4.0},
{6.0, 24.0, 36.0, 24.0, 6.0},
{4.0, 16.0, 24.0, 16.0, 4.0},
{1.0, 4.0, 6.0, 4.0, 1.0}};

#define KCENTER 2

static inline void	something(vec3 at, vec3 ij, const texture *self, color *sum)
{
	color	pixe_value;
	int		x_offset;
	int		y_offset;
	int		i;
	int		j;

	i = ij.x;
	j = ij.y;
	x_offset = at.x + i * 3 - KCENTER;
	y_offset = at.y + j * 3 - KCENTER;
	if (x_offset >= 0 && x_offset < self->image_width && y_offset >= 0
		&& y_offset < self->image_height)
	{
		pixe_value = self->value(x_offset / self->image_width, y_offset
				/ self->image_height, self);
		sum->x += g_kernel[i][j] * pixe_value.x;
		sum->y += g_kernel[i][j] * pixe_value.y;
		sum->z += g_kernel[i][j] * pixe_value.z;
	}
}

color	gaussian_blur(int x, int y, const texture *self)
{
	color	sum;
	int		i;
	int		j;

	sum = v3(0.0);
	i = 0;
	while (i < 5)
	{
		j = 0;
		while (j < 5)
		{
			something(v3(x, y), v3(i, j), self, &sum);
			j++;
		}
		i++;
	}
	return (v_scal(sum, 1.0 / 256.0f));
}

// int		ft_find(char c, char *str)
// {
// 	int	i;
//
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (c == str[i])
// 			return (i);
// 	}
// 	return (-1);
// }
//
// char	**inplace_split(char *str, char *sep)
// {
// 	char	**splits;
// 	int		c;
// 	int		i;
// 	int		previous_is;
//
// 	c = 0;
// 	previous_is = 1;
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (ft_find(str[i], sep) == -1 && previous_is)
// 			c++;
// 		previous_is = ft_find(str[i], sep) == -1;
// 		i++;
// 	}
// 	splits = gc_malloc(sizeof(char *) * (c + 1));
// 	previous_is = 1;
// 	c = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (ft_find(str[i], sep) == -1 && previous_is)
// 			splits[c++] = ft_strdup()
// 		previous_is = ft_find(str[i], sep) == -1;
// 		i++;
// 	}
// }

char	**_gc_add_2d(char	**ptr_2d)
{
	int	i;

	garbage_collector(ADD, ptr_2d);
	i = 0;
	while (ptr_2d[i] != NULL)
		garbage_collector(ADD, ptr_2d[i++]);
	return (ptr_2d);
}
