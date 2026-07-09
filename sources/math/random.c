
#include "fractol.h"

// 0.0 to 1.0 using PCG hash
PFPN	random_pfpn(void)
{
	static unsigned int	state;
	unsigned int		result;

	state = state * 747796405 + 2891336453;
	result = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
	result = (result >> 22) ^ result;
	return (result / 4294967295.0);
}

PFPN	random_normal_distribution(void)
{
	PFPN	theta;
	PFPN	rho;

	theta = 2 * 3.1415926 * random_pfpn();
	rho = sqrt(-2 * log(random_pfpn()));
	return (rho * cos(theta));
}

PFPN	random_pfpn_l(PFPN min, PFPN max)
{
	return (min + (max - min) * random_pfpn());
}

vec3	pixel_sample_square(void)
{
	PFPN	px;
	PFPN	py;

	px = -0.5 + random_pfpn();
	py = -0.5 + random_pfpn();
	return (v_add(v_scal(v.pixel_delta_u, px), v_scal(v.pixel_delta_v, py)));
}

vec3	random_in_unit_disk(void)
{
	vec3	p;

	while (True)
	{
		p = v3(random_pfpn() * 2 - 1, random_pfpn() * 2 - 1, 0);
		if (length_squared(p) < 1)
			return (p);
	}
	return (v3(0, 0, 0));
}

// Returns a random point in the camera defocus disk.
vec3	defocus_disk_sample(void)
{
	vec3	p;

	p = random_in_unit_disk();
	return (v_add(v_add(v.camera_pos, v_scal(v.defocus_disk_u, p.x)),
			v_scal(v.defocus_disk_v, p.y)));
}

vec3	random_v3(void)
{
	return (v3(random_pfpn(), random_pfpn(), random_pfpn()));
}

vec3	random_v3_l(PFPN min, PFPN max)
{
	return (v3(random_pfpn_l(min, max), random_pfpn_l(min, max),
			random_pfpn_l(min, max)));
}

vec3	random_in_unit_sphere(void)
{
	vec3	p;

	while (True)
	{
		p = random_v3_l(-1, 1);
		if (length_squared(p) < 1)
			return (p);
	}
}

vec3	random_unit_vector(void)
{
	return (v_norm(random_in_unit_sphere()));
}

// // In the same hemisphere as the normal
// vec3	random_on_hemisphere(vec3 normal)
// {
// 	vec3	on_unit_sphere;

// 	on_unit_hemisphere = random_unit_vector();
// 	if (v_dot(on_unit_sphere, normal) > 0.0)
// 		return (on_unit_sphere);
// 	else
// 		return (v_scal(on_unit_sphere, -1));
// }