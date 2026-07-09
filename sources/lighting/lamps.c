
#include "fractol.h"

t_item	*add_item(vec3 position, vec3 scale, vec3 rotation, material m,
		void (*raster)(t_item *), bool (*hit)(const ray *, const interval,
			hit_record *, const t_item *))
{
	return (add_item_((t_item){(tfm){position, rotation, scale}, m, raster,
		hit}));
}

t_item	*add_item_(t_item t)
{
	if (v.item_count >= MAX_ITEM_COUNT)
	{
		printf("Too many items >%d, recompile with greater MAX_ITEM_COUNT\n",
			MAX_ITEM_COUNT);
		return (NULL);
	}
	v.items[v.item_count] = t;
	return (&(v.items[v.item_count++]));
}

void	remove_item(t_item *t_ptr)
{
	t_item	*last;

	last = &(v.items[v.item_count - 1]);
	memcpy(t_ptr, last, sizeof(t_item));
	*last = (t_item){};
	v.item_count--;
}

t_light	*add_lamp(color col, vec3 pos_dir, PFPN intensity, bool is_dir)
{
	if (is_dir)
	{
		return (add_lamp_((t_light){(tfm){v3(), v3(), v_3(intensity)}, col,
			v_norm(pos_dir), intensity, True}));
	}
	else
		return (add_lamp_((t_light){(tfm){pos_dir, v3(), v_3(intensity)}, col,
			pos_dir, intensity, False}));
}

t_light	*add_lamp_(t_light l)
{
	l.transform.scale = v_3(l.intensity);
	if (l.is_dir)
		l.transform.rot = dir_to_rot(l.dir);

	if (v.light_count >= MAX_LIGHT_COUNT)
	{
		printf("Too many lamps >%d, recompile with greater MAX_LIGHT_COUNT\n",
			MAX_LIGHT_COUNT);
		return (NULL);
	}
	v.lights[v.light_count] = l;
	return (&(v.lights[v.light_count++]));
}


void	remove_lamp(t_light *l_ptr)
{
	t_light	*last;

	last = &(v.lights[v.light_count - 1]);
	memcpy(l_ptr, last, sizeof(t_light));
	*last = (t_light){};
	v.light_count--;
}

t_item	get_item_default(void)
{
	t_item	get;

	get = (t_item){(tfm){v3(0), v3(0), v3(1, 1, 1)}};
	get.mat = new_m(random_unit_vector());
	if (v._1)
	{
		get.raster = raster_sphere;
		get.hit = hit_sphere;
	}
	else if (v._2)
	{
		get.raster = raster_box;
		get.hit = hit_box;
	}
	else if (v._3)
	{
		get.raster = raster_cylinder;
		get.hit = hit_cylinder;
	}
	else if (v._4)
	{
		get.raster = raster_pyramid;
		get.hit = hit_cone;
	}
	return (get);
}

t_light	get_lamp_default(void)
{
	t_light	get;

	get = (t_light){};
	get.transform = (tfm){v3(0), v3(0), v3(1, 1, 1)};
	get.col = WHITE;
	get.dir = v3(0, -1, 0);
	get.intensity = 1.0;
	get.is_dir = False;
	if (v._5)
	{
		get.is_dir = False;
	}
	else if (v._6)
	{
		get.is_dir = True;
	}
	return (get);
}
