
#include "fractol.h"

vec3	v_add(vec3 a, vec3 b)
{
	return (v3(a.x + b.x, a.y + b.y, a.z + b.z));
}

vec3	from_to(vec3 a, vec3 b)
{
	return (v_sub(b, a));
}

vec3	v_sub(vec3 a, vec3 b)
{
	return (v3(a.x - b.x, a.y - b.y, a.z - b.z));
}

vec3	v_mult(vec3 a, vec3 b)
{
	return (v3(a.x * b.x, a.y * b.y, a.z * b.z));
}

vec3	v_div(vec3 a, vec3 b)
{
	return (v3(a.x / b.x, a.y / b.y, a.z / b.z));
}

// defaults to 0
inline vec3	v3_base(vec3 in)
{
	return ((vec3){in.x, in.y, in.z});
}

// all values to X
vec3	v_3(PFPN x)
{
	return (v3(x, x, x));
}

bool	v_eq(vec3 a, vec3 b)
{
	return (close_enough(a.x - b.x) && close_enough(a.y - b.y)
		&& close_enough(a.z - b.z));
}

vec3	v_norm(vec3 a)
{
	PFPN	len;

	len = v_len(a);
	return ((vec3){a.x / len, a.y / len, a.z / len});
}

vec3	swap_yz(vec3 v)
{
	return (v3(v.x, v.z, v.y));
}

int	in_bounds(int x, int y)
{
	if (x < 0 || y < 0 || x >= v.w || y >= v.h)
		return (0);
	return (1);
}

int	v_in_bounds(vec3 pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.x >= v.w || pos.y >= v.h)
		return (0);
	return (1);
}

PFPN	length_squared(vec3 a)
{
	return (a.x * a.x + a.y * a.y + a.z * a.z);
}

PFPN	v_len(vec3 a)
{
	return (sqrtf(a.x * a.x + a.y * a.y + a.z * a.z));
}

/* distance between 2 points */
PFPN	vec_dist(vec3 a, vec3 b)
{
	return (v_len(v_sub(a, b)));
}

vec3	v_scal(vec3 a, PFPN scalar)
{
	return ((vec3){a.x * scalar, a.y * scalar, a.z * scalar});
}

PFPN	v_dot(vec3 a, vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

vec3	v_cross(vec3 a, vec3 b)
{
	vec3	cp;

	cp.x = a.y * b.z - a.z * b.y;
	cp.y = a.z * b.x - a.x * b.z;
	cp.z = a.x * b.y - a.y * b.x;
	return (cp);
}

vec3	line_intersection(vec3 A, vec3 B, vec3 C, vec3 D)
{
	PFPN	a[2];
	PFPN	b[2];
	PFPN	c[2];
	PFPN	determinant;

	a[0] = B.y - A.y;
	b[0] = A.x - B.x;
	c[0] = a[0] * (A.x) + b[0] * (A.y);
	a[1] = D.y - C.y;
	b[1] = C.x - D.x;
	c[1] = a[1] * (C.x) + b[1] * (C.y);
	determinant = a[0] * b[1] - a[1] * b[0];
	if (determinant == 0)
	{
		return (v3(-1, -1));
	}
	else
	{
		return (v3((b[1] * c[0] - b[0] * c[1]) / determinant,
				(a[0] * c[1] - a[1] * c[0]) / determinant));
	}
}

// sometimes gives wrong Z distance, PROBABLY WHEN one point is behind,
// or smth similar
vec3	project_onto_screen_limits(vec3 p, vec3 dir)
{
	vec3	r;
	vec3	intersections[2];

	intersections[0] = v3(-1, -1);
	intersections[1] = v3(-1, -1);
	if (dir.x > 0)
		intersections[0] = line_intersection(p, v_add(p, dir), v3(v.w - 1, 0),
				v3(v.w - 1, 1));
	else if (dir.x < 0)
		intersections[0] = line_intersection(p, v_add(p, dir), v3(0, 0), v3(0,
					1));
	if (dir.y < 0)
		intersections[1] = line_intersection(p, v_add(p, dir), v3(0), v3(1));
	else if (dir.y > 0)
		intersections[1] = line_intersection(p, v_add(p, dir), v3(0, v.h - 1),
				v3(1, v.h - 1));
	r = intersections[v_in_bounds(intersections[1])];
	if (dir.x != 0)
		r.z = (r.x - p.x) / dir.x;
	else
		r.z = (r.y - p.y) / dir.y;
	if (r.z < 0)
		r.z = 0;
	return (r);
}

//parent V to the ITEM
vec3	parent_to(vec3 v, const tfm *transform)
{
	v = v_mult(v, transform->scale);
	v = rotate3(v, transform->rot);
	v = v_add(v, transform->pos);
	return (v);
}

vec3	parent_to_virtual(vec3 v, vec3 pos, vec3 rot, vec3 scale)
{
	v = v_mult(v, scale);
	v = rotate3(v, rot);
	v = v_add(v, pos);
	return (v);
}

vec3	project_onto_line(vec3 A, vec3 B, vec3 C)
{
	vec3	_ab;
	vec3	_ac;
	vec3	_p;
	PFPN	dot_ab;
	PFPN	dot_ac;

	_ab.x = B.x - A.x;
	_ab.y = B.y - A.y;
	_ac.x = C.x - A.x;
	_ac.y = C.y - A.y;
	dot_ab = _ab.x * _ab.x + _ab.y * _ab.y;
	dot_ac = _ac.x * _ab.x + _ac.y * _ab.y;
	_p.x = (dot_ac / dot_ab) * _ab.x;
	_p.y = (dot_ac / dot_ab) * _ab.y;
	return (v_add(_p, A));
}

PFPN	tddd(vec3 from, vec3 to, vec3 dir)
{
	vec3	v;
	PFPN	d;

	v = from_to(from, to);
	d = sqrt(v.x * v.x + v.y * v.y);
	if (v_dot(v, dir) < 0)
		return (-d);
	return (d);
}

// in Radians
//[0, MYPI]
PFPN	get_angle(vec3 dir)
{
	PFPN	a;

	a = atan2(dir.y, dir.x);
	if (a < 0)
		return (a + MYPI);
	return (a);
}

void	v_round(vec3 *in, PFPN precision)
{
	in->x = (int)(in->x / precision) * precision;
	in->y = (int)(in->y / precision) * precision;
	in->z = (int)(in->z / precision) * precision;
}

// PFPN directioned_distance(vec3 from, vec3 to, vec3 dir)
// {
// 	PFPN d = vec_dist(from, to);

// 	vec3 my_dir = from_to(from, to);
// 	if (signd(my_dir.x) != signd(dir.x) && signd(my_dir.y) != signd(dir.y))
// 		return (-d);
// 	return (d);
// }
// PFPN distanceToLine(vec3 A, vec3 B, vec3 C) {
//     PFPN numerator = fabs((B.x - A.x) * (A.y - C.y) - (A.x - C.x) * (B.y
// - A.y));
//     PFPN denominator = sqrt((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y
// - A.y));
//     PFPN distance = numerator / denominator;
//     return (distance);
// }
