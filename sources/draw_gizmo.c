/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draws.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	gizmo_nshape(int n, vec3 pos, vec3 rot, vec3 scale, double r_offset, color c)
{
	double	_a = 0 + r_offset;// 45'
	vec3 this = v3(scale.x*sin(_a), scale.y*cos(_a));
	this = rotate3(this, rot);
	for (int i=1; i <= n; i++)
	{
		_a = MYPI*2/n*i + r_offset;// 45', 135', 225', 315', 45'
		vec3 next = v3(scale.x*sin(_a), scale.y*cos(_a));
		next = rotate3(next, rot);
		gizmo_line(v_add(pos, this), v_add(pos, next), c);
		this = next;
	}
}

void	gizmo_dot(vec3 pos, color c)
{
	for (int x=-2; x<=2; x++)
		for (int y=-2; y<=2; y++)
			if (!((x==-2||x==2) && (y==-2||y==2)))
				draw_inbounds(pos.x+x, pos.y+y, c);
}

static void	draw_line(vec3 start, vec3 end, color c, int	(*draw_func)(vec3 p, color c))
{
	if (v_in_bounds(end) && !v_in_bounds(start))
		return (gizmo_line(end, start, c));
	
	vec3 move = v_sub(end, start);
	vec3 step;
	if (fabs(move.x)>=fabs(move.y))
		step = v3(1 * signd(move.x), move.y/fabs(move.x), move.z/fabs(move.x));
	else
		step = v3(move.x/fabs(move.y), 1 * signd(move.y), move.z/fabs(move.y));
	vec3 ve = start;
	while (draw_func(ve, c) && (fabs(ve.x-end.x) > 1 || fabs(ve.y-end.y) > 1))
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

void	gizmo_box(vec3 pos, int w, int h, color c)
{
	for (int x=-w; x<=w; x++)
		for (int y=-h; y<=h; y++)
			draw_inbounds(pos.x+x, pos.y+y, c);
	(void) gizmo_box;
}

void	gizmo_drag(vec3 start_pos, vec3 current_pos, color c)
{
	scribe_contour("O", start_pos.x-2, start_pos.y+4, c, WHITE);
	scribe_contour("x", current_pos.x-2, current_pos.y+4, c, WHITE);
	draw_debug_line(start_pos, current_pos, WHITE);
}

void	draw_debug_line(vec3 start, vec3 end, color c)
{
	if (in_bounds(end.x, end.y) && !in_bounds(start.x, start.y))
	{
		draw_debug_line(end, start, c);
		return;
	}
	if (start.x == end.x) {start.x += 0.000001;}
	
	vec3 move = v3(end.x - start.x, end.y - start.y);
	vec3 step;
	if (fabs(move.x)>=fabs(move.y))
		step = v3(sign(move.x), move.y/fabs(move.x));
	else
		step = v3(move.x/fabs(move.y), sign(move.y));
	vec3 ve = start;
	while (in_bounds(ve.x, ve.y) && (fabs(end.x-ve.x)>fabs(step.x)))
	{
		mlx_pixel_put(v.mlx, v.win, ve.x, ve.y, color2rgb(c));
		ve.x += step.x;
		ve.y += step.y;
	}
	if (in_bounds(ve.x, ve.y))
		mlx_pixel_put(v.mlx, v.win, ve.x, ve.y, color2rgb(c));
	if (in_bounds(end.x, end.y))
		mlx_pixel_put(v.mlx, v.win, end.x, end.y, color2rgb(c));
}

void	draw_debug_dot(vec3 pos, color c)
{
	for (int x=-2; x<=2; x++)
		for (int y=-2; y<=2; y++)
			if (!((x==-2||x==2) && (y==-2||y==2)))
				if (in_bounds(pos.x+x, pos.y+y))
					mlx_pixel_put(v.mlx, v.win, pos.x+x, pos.y+y, color2rgb(c));
}

// -- PROJECTED

//SEGV so much
void	draw_projected_line(vec3 start, vec3 end, vec3 c)
{
	start = world_to_screenpos(start);
	end = world_to_screenpos(end);

	//2 behind
	if (start.z < 0 && end.z < 0) return;
	//throw out some useless cases

	//idk why but this does fix the final issues, just leave this swap here
	if (end.z < 0){
		vec3 temp = start;
		start = end;
		end = temp;
	}

	if (start.z > 0 && end.z > 0
	    &&((start.x <= 0   && end.x <= 0)
		|| (start.x >= v.w && end.x >= v.w)
		|| (start.y <= 0   && end.y <= 0)
		|| (start.y >= v.h && end.y >= v.h)))
		return;


	if (!v_in_bounds(start) || (start.z < 0 && !v.orthographic_toggle))
	{
		start = project_onto_screen_limits(start, v_scal(v_sub(start, end), signd(start.z)));
	}
	if (!v_in_bounds(end) || (end.z < 0 && !v.orthographic_toggle))
	{
		end = project_onto_screen_limits(end, v_scal(v_sub(end, start), signd(end.z)));
	}

	start.x = (int)start.x; start.y = (int)start.y;
	end.x = (int)end.x; end.y = (int)end.y;
	if (v_in_bounds(start) && v_in_bounds(end))
		heat_line(start, end, c);
}

void	draw_projected_dot(vec3 pos, color c)
{
	pos = world_to_screenpos(pos);
	vec3 _p = v3(0,0,pos.z);
	for (int x=-2; x<=2; x++)
		for (int y=-2; y<=2; y++)
		{
			if ((x==-2||x==2) && (y==-2||y==2)) continue;
			_p.x = (int)(pos.x+x), _p.y = (int)(pos.y+y);
			draw_v_inheat(_p, c);
		}
}

void	draw_projected_point(vec3 pos, color c)
{
	pos = world_to_screenpos(pos);
	draw_v_inheat(pos, c);
}