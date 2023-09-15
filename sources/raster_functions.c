/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raster_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

//parent V to the ITEM
vec3	parent_to(vec3 v, t_item *item)
{
	v = v_mult(v, item->scale);
	v = rotate3(v, item->rot);
	v = v_add(v, item->pos);
	return (v);
}

vec3	parent_to_virtual(vec3 v, vec3 pos, vec3 rot, vec3 scale)
{
	v = v_mult(v, scale);
	v = rotate3(v, rot);
	v = v_add(v, pos);
	return (v);
}

void	raster_cylinder(t_item *item)
{
	int		v_splits = 16;

	vec3	*l1 = get_npoints(v_splits, 0);
	vec3	*l2 = get_npoints(v_splits, 0);

	for (int j=0; j<=v_splits; j++) l1[j] = v_add(l1[j], v3(0, 0,  1));
	for (int j=0; j<=v_splits; j++) l2[j] = v_add(l2[j], v3(0, 0, -1));

	for (int j=0; j<=v_splits; j++) l1[j] = parent_to(l1[j], item);
	for (int j=0; j<=v_splits; j++) l2[j] = parent_to(l2[j], item);

//TOP & BOT
	for (int j=0; j<v_splits; j++) draw_projected_line(l1[j], l1[j+1], item->mat.albedo.color_value);
	for (int j=0; j<v_splits; j++) draw_projected_line(l2[j], l2[j+1], item->mat.albedo.color_value);

//draw joints
	for (int j=0; j<v_splits; j++) draw_projected_line(l1[j], l2[j], item->mat.albedo.color_value);

	free(l1);
	free(l2);
}

vec3	swap_yz(vec3 v)
{
	return v3(v.x, v.z, v.y);
}

void	raster_sphere(t_item *item)
{
	int	h_splits = 16;	//segments	-min: 3
	int	v_splits = 8;	//rings		-min: 3

	vec3	north_pole = v3(0,  1, 0);
	vec3	south_pole = v3(0, -1, 0);

	north_pole = parent_to(north_pole, item);
	south_pole = parent_to(south_pole, item);

	vec3	*ring_scale = get_npoints(2*v_splits, 0);
	for (int j=0; j<=2*v_splits; j++) ring_scale[j] = swap_yz(ring_scale[j]);


	vec3	**rings = malloc((v_splits-1) * sizeof(vec3 *));
	for (int j=1; j<v_splits; j++)
	{
		rings[j-1] = get_npoints(h_splits, 0);
		for (int k=0; k<=h_splits; k++) {rings[j-1][k].z = 1; rings[j-1][k] = swap_yz(rings[j-1][k]); }
		
		vec3 new_scale = v3(item->scale.x * ring_scale[j].x,
							item->scale.y * ring_scale[j].z,
							item->scale.z * ring_scale[j].x);//= v_mult(item->scale, ring_scale[j]);
		for (int k=0; k<=h_splits; k++) rings[j-1][k] = parent_to_virtual(rings[j-1][k], item->pos, item->rot, new_scale);
		//circle
		for (int k=0; k<h_splits; k++)  draw_projected_line(rings[j-1][k], rings[j-1][k+1], item->mat.albedo.color_value);
		//joints
		if (j > 1)
			for (int k=0; k<h_splits; k++)  draw_projected_line(rings[j-1][k], rings[j - 2][k], item->mat.albedo.color_value);
	}
	for (int k=0; k<h_splits; k++)  draw_projected_line(rings[0][k], north_pole, item->mat.albedo.color_value);
	for (int k=0; k<h_splits; k++)  draw_projected_line(rings[v_splits-2][k], south_pole, item->mat.albedo.color_value);

	if (v._debug)
	{
		draw_projected_dot(north_pole, RED);
		draw_projected_dot(south_pole, BLUE);
	}

	free(ring_scale);
	for (int k=0; k<v_splits-1; k++)  free(rings[k]);
	free(rings);
}

void	raster_pyramid(t_item *item)
{
	vec3 c1, c2, c3, c4;
	
	c1 = v3( 0,  1,  0);
	c2 = v3( sin(0), 		-1, cos(0));
	c3 = v3( sin(MYPI*2/3), -1, cos(MYPI*2/3));
	c4 = v3( sin(MYPI*4/3), -1, cos(MYPI*4/3));

	c1 = parent_to(c1, item);
	c2 = parent_to(c2, item);
	c3 = parent_to(c3, item);
	c4 = parent_to(c4, item);

//top
	draw_projected_line(c1, c2, item->mat.albedo.color_value);
	draw_projected_line(c1, c3, item->mat.albedo.color_value);
	draw_projected_line(c1, c4, item->mat.albedo.color_value);
//base
	draw_projected_line(c2, c3, item->mat.albedo.color_value);
	draw_projected_line(c3, c4, item->mat.albedo.color_value);
	draw_projected_line(c4, c2, item->mat.albedo.color_value);

	if(v._debug)
	{
		draw_projected_dot(c1, RED);
		draw_projected_dot(c2, GREEN);
		draw_projected_dot(c3, BLUE);
		draw_projected_dot(c4, WHITE);
	}
}

void	raster_box(t_item *item)
{
	vec3 c1, c2, c3, c4, c5, c6, c7, c8;
	
	c1 = parent_to(v3( 1,  1,  1), item);
	c2 = parent_to(v3( 1,  1, -1), item);
	c3 = parent_to(v3( 1, -1, -1), item);
	c4 = parent_to(v3( 1, -1,  1), item);
	c5 = parent_to(v3(-1,  1,  1), item);
	c6 = parent_to(v3(-1,  1, -1), item);
	c7 = parent_to(v3(-1, -1, -1), item);
	c8 = parent_to(v3(-1, -1,  1), item);

//top
	draw_projected_line(c1, c2, item->mat.albedo.color_value);
	draw_projected_line(c2, c3, item->mat.albedo.color_value);
	draw_projected_line(c3, c4, item->mat.albedo.color_value);
	draw_projected_line(c4, c1, item->mat.albedo.color_value);
//bot
	draw_projected_line(c5, c6, item->mat.albedo.color_value);
	draw_projected_line(c6, c7, item->mat.albedo.color_value);
	draw_projected_line(c7, c8, item->mat.albedo.color_value);
	draw_projected_line(c8, c5, item->mat.albedo.color_value);
//left
	draw_projected_line(c1, c5, item->mat.albedo.color_value);
	draw_projected_line(c2, c6, item->mat.albedo.color_value);
//right
	draw_projected_line(c3, c7, item->mat.albedo.color_value);
	draw_projected_line(c4, c8, item->mat.albedo.color_value);

	if (v._debug)
	{
		draw_projected_dot(c1, rgb2color(new_rgb(200, 0, 0)));
		draw_projected_dot(c2, rgb2color(new_rgb(200, 200, 0)));
		draw_projected_dot(c3, rgb2color(new_rgb(200, 0, 200)));
		draw_projected_dot(c4, rgb2color(new_rgb(200, 200, 200)));
		draw_projected_dot(c5, rgb2color(new_rgb(0, 200, 0)));
		draw_projected_dot(c6, rgb2color(new_rgb(0, 200, 200)));
		draw_projected_dot(c7, rgb2color(new_rgb(0, 0, 200)));
		draw_projected_dot(c8, rgb2color(new_rgb(100, 200, 100)));
	}
}

void	raster_quad(t_item *item)
{
	vec3 c1 = parent_to(v3( 1, 0, 1), item);
	vec3 c2 = parent_to(v3( 1, 0,-1), item);
	vec3 c3 = parent_to(v3(-1, 0, 1), item);
	vec3 c4 = parent_to(v3(-1, 0,-1), item);

	draw_projected_line(c1, c2, item->mat.albedo.color_value);
	draw_projected_line(c2, c4, item->mat.albedo.color_value);
	draw_projected_line(c4, c3, item->mat.albedo.color_value);
	draw_projected_line(c3, c1, item->mat.albedo.color_value);

	if (1 || v._debug)
	{
		vec3 normal = rotate3(v3(0,.5,0), item->rot);
		draw_projected_line(v_add(item->pos, normal), item->pos, item->mat.albedo.color_value);
		draw_projected_dot(v_add(item->pos, normal), item->mat.albedo.color_value);
	}
}

void	raster_plane(t_item *item)
{
	vec3 c1 = parent_to(v3( 1, 0, 1), item);
	vec3 c2 = parent_to(v3( 1, 0,-1), item);
	vec3 c3 = parent_to(v3(-1, 0, 1), item);
	vec3 c4 = parent_to(v3(-1, 0,-1), item);

	draw_projected_line(c1, c2, item->mat.albedo.color_value);
	draw_projected_line(c2, c4, item->mat.albedo.color_value);
	draw_projected_line(c4, c3, item->mat.albedo.color_value);
	draw_projected_line(c3, c1, item->mat.albedo.color_value);
	draw_projected_line(c1, c4, item->mat.albedo.color_value);
	draw_projected_line(c2, c3, item->mat.albedo.color_value);

	if (1 || v._debug)
	{
		vec3 normal = rotate3(v3(0,.5,0), item->rot);
		draw_projected_line(v_add(item->pos, normal), item->pos, item->mat.albedo.color_value);
		draw_projected_dot(v_add(item->pos, normal), item->mat.albedo.color_value);
	}
}

//2d not projected

//BOX - 1
void	raster_square(t_item *item)
{
	vec3 c1, c2, c3, c4;
	
	c1 = parent_to(v3( 1,  1), item);
	c2 = parent_to(v3( 1, -1), item);
	c3 = parent_to(v3(-1,  1), item);
	c4 = parent_to(v3(-1, -1), item);

	gizmo_line(c1, c2, item->mat.albedo.color_value);
	gizmo_line(c2, c3, item->mat.albedo.color_value);
	gizmo_line(c3, c4, item->mat.albedo.color_value);
	gizmo_line(c4, c1, item->mat.albedo.color_value);
}
//CIRCLE - 2
void	raster_circle(t_item *item)
{
	gizmo_nshape(max(item->scale.x*2.0/50.0, 10), item->pos, item->rot, item->scale, 0, item->mat.albedo.color_value);
}
//TRI.. 3,4,0
void	raster_tri(t_item *item)
{
	gizmo_nshape(3, item->pos, item->rot, item->scale, MYPI, item->mat.albedo.color_value);
}
void	raster_hex(t_item *item)
{
	gizmo_nshape(6, item->pos, item->rot, item->scale, MYPI/6, item->mat.albedo.color_value);
}
void	raster_line(t_item *item)
{
	gizmo_line(item->pos,
		v3(item->pos.x+sin(item->rot.x)*item->scale.x,
			item->pos.y+cos(item->rot.x)*item->scale.x), item->mat.albedo.color_value);
}
