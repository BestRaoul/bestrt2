
#include "fractol.h"

void	add_motion(PFPN *value, PFPN start_value, PFPN end_value,
		PFPN (*tween)(PFPN, PFPN, PFPN))
{
	motion	*new_motions;
	int		i;

	new_motions = gc_malloc((v.motion_count + 1) * sizeof(motion));
	i = 0;
	while (i < v.motion_count)
	{
		new_motions[i] = v.motions[i];
		i++;
	}
	new_motions[v.motion_count] = (motion){value, start_value, end_value,
		tween};
	v.motions = new_motions;
	v.motion_count++;
}

PFPN	lerpd(PFPN a, PFPN b, PFPN t)
{
	return ((1 - t) * a + t * b);
}

PFPN	ping_pong(PFPN a, PFPN b, PFPN t)
{
	PFPN	t2;

	if (t < 0.5)
		t2 = t;
	else
		t2 = 1 - t;
	t2 *= 2;
	return ((1 - t2) * a + t2 * b);
}

PFPN	ping_pong_2(PFPN a, PFPN b, PFPN t)
{
	PFPN	t2;

	if (t < 0.5)
		t2 = t;
	else
		t2 = 1 - t;
	t2 *= 2 * 2 * t2;
	return ((1 - t2) * a + t2 * b);
}

PFPN	sin_tween(PFPN a, PFPN b, PFPN t)
{
	PFPN	angle;

	angle = t * MYPI * 2;
	t = sin(angle);
	t = t / 2.0 + .5;
	return (lerpd(a, b, t));
}

PFPN	cos_tween(PFPN a, PFPN b, PFPN t)
{
	PFPN	angle;

	angle = t * MYPI * 2;
	t = cos(angle);
	t = t / 2.0 + .5;
	return (lerpd(a, b, t));
}

PFPN	ease_in_out_cubic(PFPN a, PFPN b, PFPN t)
{
	if (t < 0.5)
		t = 4 * t * t * t;
	else
		t = 1 - pow(-2 * t + 2, 3) / 2;
	return (lerpd(a, b, t));
}

// Scale, and clamp x to 0..1 range
PFPN	smoothstep(PFPN edge0, PFPN edge1, PFPN x)
{
	x = clamp_((x - edge0) / (edge1 - edge0));
	return (x * x * (3.0 - 2.0 * x));
}

