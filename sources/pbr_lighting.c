#include "fractol.h"

double distribution_ggx(double nDotH, double roughness)
{
    double a = roughness * roughness;
    double a2 = a * a;
    double denom = nDotH * nDotH * (a2 - 1) + 1;
    denom = MYPI * denom * denom;
    return fmax(a2 / denom, 0.0000001);
}

double geometry_smith(double NdotV, double NdotL, double roughness)
{
    double r = 1.0 + roughness;
    double k = (r * r) / 8.0;
    double ggx1 = NdotV / (NdotV * (1.0 - k) + k);
    double ggx2 = NdotL / (NdotL * (1.0 - k) + k);
    return ggx1 * ggx2;
}

vec3 fresnel_shlick(double HdotV, vec3 base_reflectivity)
{
    return v_add(base_reflectivity,
            v_scal(v_sub(v_3(1), base_reflectivity), pow(1.0 - HdotV, 5)));
}

// --- VERIFIED ---

//1 on failure OBSTRUCTED
Bool    check_obstruction(t_light *l, vec3 p)
{
        hit_record ignore;
        ray check;
        double max_t = INFINITY;

        check.orig = p;
        if (l->is_dir)
            check.dir = v_scal(l->pos_dir, -1);
        else //Point light
        {
            check.dir = v_norm(v_sub(l->pos_dir, p));
            max_t = vec_dist(l->pos_dir, p);
        }
        return hit(&check, (interval){0.001, max_t}, &ignore);
}

vec3 CalcTotalPBRLighting(hit_record *rec, ray *ray_in)
{
    double  metalic     = evaluate_bw(&rec->mat.metalic, rec->u, rec->v);
    double  roughness   = evaluate_bw(&rec->mat.roughness, rec->u, rec->v);
    vec3    albedo      = evaluate(&rec->mat.base_color, rec->u, rec->v);
    vec3    base_reflectivity = lerp(metalic, v3(0, 0, 0.4), albedo); // mix base to albedo

    vec3 N = rec->normal;
    vec3 V = ray_in->dir;

    vec3 Lo = v3(0.0);
    for (int i=0; i<v.light_count; i++)
    {
        t_light *l = &v.lights[i];
        if (l->is_dir) { NOT_IMPLEMENTED("directional lights"); continue; }
        if (check_obstruction(l, rec->p)) continue;

        vec3 L = v_norm(v_sub(l->pos_dir, rec->p));
        vec3 H = v_norm(v_add(V, L));
        double distance = v_len(v_sub(l->pos_dir, rec->p));
        double attenuation = 1.0 / (distance * distance);
        vec3 radiance = v_scal(l->col, attenuation);

        //Cook-Torrence BRDF
        double NdotV = fmax(v_dot(N, V), 0.0000001);

        double NdotL = fmax(v_dot(N, L), 0.0000001);
        double HdotV = fmax(v_dot(H, V), 0.0);
        double NdotH = fmax(v_dot(N, H), 0.0);

        double D = distribution_ggx(NdotH, roughness);
        double G = geometry_smith(NdotV, NdotL, roughness);
        vec3   F = fresnel_shlick(HdotV, base_reflectivity);

        vec3 specular = v_scal(F, D * G); // D * G * F
        specular = v_scal(specular, 1.0 / (4.0 * NdotV * NdotL));

        //conservation of energy
        vec3 kd = v_sub(v_3(1.0), F);

        //adapt to metalic
        kd = v_scal(kd, 1.0 - metalic);

        //combine
        Lo = v_add(Lo, v_mult(
                v_add(v_scal(v_mult(kd, albedo), MYPI), specular),
                v_scal(radiance, NdotL)));
    }

    vec3 ambient = v_scal(albedo, 0.03);

    vec3 color = v_add(ambient, Lo);

    //HDR tone mapping
    color = v_div(color, v_add(color, v_3(1.0)));

    //gamma correct
    color = v3(pow(color.x, 1.0/2.2), pow(color.y, 1.0/2.2), pow(color.z, 1.0/2.2));

    return color;
}