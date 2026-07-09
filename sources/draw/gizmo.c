
#include "fractol.h"

void	gizmo_dot(vec3 pos, color c)
{
	int	x;
	int	y;

	x = -2;
	while (x <= 2)
	{
		y = -2;
		while (y <= 2)
		{
			if (!((x == -2 || x == 2) && (y == -2 || y == 2)))
				draw_inbounds(pos.x + x, pos.y + y, c);
			y++;
		}
		x++;
	}
}

void	draw_line(vec3 start, vec3 end, color c, int (*draw_func)(vec3 p,
			color c))
{
	vec3	move;
	vec3	step;
	vec3	ve;

	if (v_in_bounds(end) && !v_in_bounds(start))
		return (draw_line(end, start, c, draw_func));
	move = v_sub(end, start);
	if (fabs(move.x) >= fabs(move.y))
		step = v3(1 * signd(move.x), move.y / fabs(move.x), move.z
				/ fabs(move.x));
	else
		step = v3(move.x / fabs(move.y), 1 * signd(move.y), move.z
				/ fabs(move.y));
	ve = start;
	while (draw_func(ve, c) && (fabs(ve.x - end.x) > 1 || fabs(ve.y
				- end.y) > 1))
	{
		ve = v_add(ve, step);
	}
	draw_func(ve, c);
	draw_func(end, c);
}

void	gizmo_line(vec3 start, vec3 end, color c)
{
	draw_line(start, end, c, draw_v_inbounds);
}

void	heat_line(vec3 start, vec3 end, color c)
{
	draw_line(start, end, c, draw_v_inheat);
}

void	gizmo_drag(vec3 start_pos, vec3 current_pos, color c)
{
	scribe("O", v3(start_pos.x - 2, start_pos.y + 4), c);
	scribe("x", v3(current_pos.x - 2, current_pos.y + 4), c);
	draw_debug_line(start_pos, current_pos, WHITE);
}

// void	gizmo_nshape(int n, vec3 pos, vec3 rot, vec3 scale, PFPN r_offset,
// 		color c)
// {
// 	vec3	this;
// 	vec3	next;

// 	PFPN _a = 0 + r_offset; // 45'
// 	this = v3(scale.x * sin(_a), scale.y * cos(_a));
// 	this = rotate3(this, rot);
// 	for (int i = 1; i <= n; i++)
// 	{
// 		_a = MYPI * 2 / n * i + r_offset; // 45', 135', 225', 315', 45'
// 		next = v3(scale.x * sin(_a), scale.y * cos(_a));
// 		next = rotate3(next, rot);
// 		gizmo_line(v_add(pos, this), v_add(pos, next), c);
// 		this = next;
// 	}
// }
// void	gizmo_box(vec3 pos, int w, int h, color c)
// {
// 	for (int x = -w; x <= w; x++)
// 		for (int y = -h; y <= h; y++)
// 			draw_inbounds(pos.x + x, pos.y + y, c);
// 	(void)gizmo_box;
// }


static int	debug_pixel(vec3 p, color c)
{
	if (!in_bounds(p.x, p.y))
		return (0);
	mlx_pixel_put(v.mlx, v.win, p.x, p.y, color2rgb(c));
	return (1);
}

void	draw_debug_line(vec3 start, vec3 end, color c)
{
	draw_line(start, end, c, debug_pixel);
}

void	draw_debug_dot(vec3 pos, color c)
{
	int	x;
	int	y;

	x = -2;
	while (x <= 2)
	{
		y = -2;
		while (y <= 2)
		{
			if (!((x == -2 || x == 2) && (y == -2 || y == 2)))
				if (in_bounds(pos.x + x, pos.y + y))
					mlx_pixel_put(v.mlx, v.win, pos.x + x, pos.y + y,
						color2rgb(c));
			y++;
		}
		x++;
	}
}

// -- PROJECTED

// line onto screen limits
// void	draw_projected_line(vec3 start, vec3 end, vec3 c)
// {
// 	vec3	dir;

// 	start = world_to_screenpos(start);
// 	end = world_to_screenpos(end);
// 	dir = from_to(start, end);
// 	heat_line(start, end, c);
// }

void	draw_projected_dot(vec3 pos, color c)
{
	vec3	_p;
	int		x;
	int		y;

	pos = world_to_screenpos(pos);
	_p = v3(0, 0, pos.z);
	x = -2;
	while (x <= 2)
	{
		y = -2;
		while (y <= 2)
		{
			if ((x == -2 || x == 2) && (y == -2 || y == 2))
			{
				y++;
				continue ;
			}
			_p.x = (int)(pos.x + x);
			_p.y = (int)(pos.y + y);
			draw_v_inheat(_p, c);
			y++;
		}
		x++;
	}
}

// void	draw_projected_point(vec3 pos, color c)
// {
// 	pos = world_to_screenpos(pos);
// 	draw_v_inheat(pos, c);
// }

// // SEGV so much
void	draw_projected_line(vec3 start, vec3 end, vec3 c)
{
	start = world_to_screenpos(start);
	end = world_to_screenpos(end);
	if (start.z < 0 && end.z < 0)
		return ;
	if (end.z < 0)
		vswap(&start, &end);
	if (start.z > 0 && end.z > 0 && ((start.x <= 0 && end.x <= 0)
			|| (start.x >= v.w && end.x >= v.w) || (start.y <= 0 && end.y <= 0)
			|| (start.y >= v.h && end.y >= v.h)))
		return ;
	if (!v_in_bounds(start) || (start.z < 0 && !v.orthographic_toggle))
	{
		start = project_onto_screen_limits(start, v_scal(v_sub(start, end),
					signd(start.z)));
	}
	if (!v_in_bounds(end) || (end.z < 0 && !v.orthographic_toggle))
	{
		end = project_onto_screen_limits(end, v_scal(v_sub(end, start),
					signd(end.z)));
	}
	start = (vec3){(int)start.x, (int)start.y};
	end = (vec3){(int)end.x, (int)end.y};
	if (v_in_bounds(start) && v_in_bounds(end) && !v_eq(start, end))
		heat_line(start, end, c);
}
