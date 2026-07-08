
#include "fractol.h"

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
