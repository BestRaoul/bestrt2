
#include "fractol.h"

typedef struct s_sim_hit {
    Bool    simd;
	vec3	p;
	color	c;
	hit_record rec;
} sim_hit;

void	simulate_ray(ray *r, sim_hit *hits, int depth)
{
    hits[0] = (sim_hit){1, r->orig, WHITE};


    for (int bounce=1; bounce<depth; bounce++) { hits[bounce].simd = False; }

    for (int bounce=1; bounce<depth; bounce++)
    {
        hits[bounce].simd = True;

        hit_record rec = (hit_record){};
        Bool did_hit = hit(r, (interval){0.001, INFINITY}, &rec);


        if (did_hit)
        {
            ray scattered;
            Bool _;
	        PBR_scatter(r, &rec, &scattered, &_);
            hits[bounce].p = scattered.orig;
            hits[bounce].c = rec.mat.base_color.color_value;
            hits[bounce].rec = rec;

            *r = scattered;
        }
        else
        {
            hits[bounce].p = v_add(r->orig, v_scal(r->dir, .2));
            hits[bounce].c = ERROR_CYAN;
            hits[bounce].rec = rec;
            break;
        }
    }
}


void    simulate_rayzz()
{
	int depth = v.max_depth+2;
	static sim_hit hits[10];

    if (v._ctrl) {
        ray r;
		init_ray(v.mouse_pos.x, v.mouse_pos.y, &r);
        simulate_ray(&r, hits, depth);
    }

    for (int i=0; i<depth && hits[i].simd; i++)
    {
        sim_hit *h = &(hits[i]);
        draw_projected_dot(h->p, h->c);
        // if (i+1<depth && hits[i+1].simd) draw_projected_line(h->p, hits[i+1].p, h->c);
		draw_projected_line(h->p, v_add(h->p, v_scal(h->rec.normal, .4)), h->rec.normal);
		if (v.light_count > 0)
		{
			t_light *l = &(v.lights[0]);
			ray to_light = (ray){h->p, v_norm(from_to(h->p, l->pos))};
			hit_record rec;
			Bool did_hit = hit(&to_light, (interval){0.0001, vec_dist(l->pos, h->p)}, &rec);

			vec3 oi = h->p;
			if (did_hit)
			{
        		draw_projected_dot(rec.p, RED);
        		draw_projected_line(h->p, rec.p, RED);
				oi = rec.p;
			}
			draw_projected_line(oi, l->pos, WHITE);
		}
    }
}

/*
{

	for (int i=0; i<=sim_depth; i++)
	{
		draw_projected_dot(hits[i].p, c);
		if (i!= sim_depth)
		{
			draw_projected_line(hits[i].p, hits[i+1].p, c);
			draw_projected_line(hits[i].p, v_add(hits[i].p, hits[i].rec.normal), BLUE);
			draw_projected_dot(hits[i+1].p, RED);
		}
		c = hits[i].c;
		c = WHITE;
		if (c.z)
		{
			//draw_projected_dot(hits[i+1].p, c);
			draw_projected_line(hits[i+1].p, hits[i+2].p, c);
			break;
		}
	}



}

{
	static hit_record rec = (hit_record){};
	static Bool	did_hit = False;
	static vec3 old_normal;
	static vec3 new_normal;

	if (v._shift)
	{
		ray shoot;
		init_ray(v.mouse_pos.x, v.mouse_pos.y, &shoot);
		did_hit = hit(&shoot, (interval){0.001, INFINITY}, &rec);

		old_normal = rec.normal;
		// maybe_apply_perturb(&rec);
		// new_normal = rec.normal;
	}
	if (did_hit)
	{
		vec3 from = rec.p; from = world_to_screenpos(from);
		vec3 to_old = v_add(rec.p, v_scal(old_normal, .2)); 	to_old = world_to_screenpos(to_old);
		vec3 to_old_h = v_add(rec.p, v_scal(old_normal, .1)); 	to_old_h = world_to_screenpos(to_old_h);
		vec3 to_new = v_add(rec.p, v_scal(new_normal, .2)); 	to_new = world_to_screenpos(to_new);

		color pink = v3(1, .2, 0);
		color green = evaluate(&(rec.mat.normal), rec.u, rec.v);
		draw_debug_dot(from, pink);
		draw_debug_dot(to_old, pink);
		draw_debug_dot(to_old_h, pink);
		draw_debug_line(from, to_old, pink);

		draw_debug_dot(to_new, green);
		draw_debug_line(from, to_new, green);
	}
}
*/

