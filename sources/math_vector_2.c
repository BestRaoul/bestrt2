
#include "fractol.h"

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
