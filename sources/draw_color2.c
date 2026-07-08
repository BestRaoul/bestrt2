
#include "fractol.h"

int		get_r(int trgb);
int		get_g(int trgb);
int		get_b(int trgb);

color	rgb2color(int rgb)
{
	return (new_color(get_r(rgb) / 256.0, get_g(rgb) / 256.0, get_b(rgb)
			/ 256.0));
}

int	get_r(int trgb)
{
	return ((trgb >> 16) & 0xFF);
}

int	get_g(int trgb)
{
	return ((trgb >> 8) & 0xFF);
}

int	get_b(int trgb)
{
	return ((trgb >> 0) & 0xFF);
}
