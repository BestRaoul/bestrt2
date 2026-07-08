
#include "fractol.h"

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
