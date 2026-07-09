
#include "fractol.h"

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

PFPN	mind(PFPN a, PFPN b)
{
	if (a > b)
		return (a);
	return (b);
}

PFPN	maxd(PFPN a, PFPN b)
{
	if (a > b)
		return (a);
	return (b);
}

PFPN	fclamp(PFPN x, PFPN min, PFPN max)
{
	if (x < min)
		return (min);
	else if (x > max)
		return (max);
	return (x);
}

int	sign(int n)
{
	if (n >= 0)
		return (1);
	return (-1);
}

PFPN	signd(PFPN n)
{
	if (n < 0)
		return (-1);
	return (1);
}

// 0.0: a
// 1.0: b
vec3	lerp(PFPN t, vec3 a, vec3 b)
{
	return ((vec3){(1. - t) * a.x + t * b.x, (1. - t) * a.y + t * b.y, (1. - t)
		* a.z + t * b.z});
}

bool	contains(interval _t, PFPN x)
{
	return (_t.min <= x && x <= _t.max);
}

bool	surrounds(interval _t, PFPN x)
{
	return (_t.min < x && x < _t.max);
}

PFPN	clamp(interval _t, PFPN x)
{
	if (x < _t.min)
		return (_t.min);
	if (x > _t.max)
		return (_t.max);
	return (x);
}

/* 0 to 1 */
PFPN	clamp_(PFPN x)
{
	if (x < 0.0)
		return (0.0);
	if (x > 1.0)
		return (1.0);
	return (x);
}

// Define the constants for the ACES tone mapping function
#define A 2.51
#define B 0.03
#define C 2.43
#define D 0.59
#define E 0.14

// ACES tone mapping function
PFPN	aces(PFPN x)
{
	return ((x * (A * x + B)) / (x * (C * x + D) + E));
}

PFPN	linear_to_gamma(PFPN linear_component)
{
	return (sqrt(linear_component));
}

bool	near_zero(vec3 e)
{
	PFPN	s;

	s = 1e-8;
	return ((fabs(e.x) < s) && (fabs(e.y) < s) && (fabs(e.z) < s));
}

bool	close_enough(PFPN __x)
{
	return (fabs(__x) < 1e-8);
}


