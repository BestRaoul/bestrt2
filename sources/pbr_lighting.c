#include "fractol.h"

//proportion of facets alligned with light
double distribution_ggx(double nDotH, double roughness)
{
    double a = roughness * roughness;
    double a2 = a * a;
    double denom = nDotH * nDotH * (a2 - 1) + 1;
    denom = MYPI * denom * denom;
    return fmax(a2 / denom, 0.0000001);
}

//self-shadowing
double geometry_smith(double NdotV, double NdotL, double roughness)
{
    double r = 1.0 + roughness;
    double k = (r * r) / 8.0;
    double ggx1 = NdotV / (NdotV * (1.0 - k) + k);
    double ggx2 = NdotL / (NdotL * (1.0 - k) + k);
    return ggx1 * ggx2;
}

//
vec3 fresnel_shlick(double HdotV, double R0)
{
    return v_3(10*R0*(R0 + (1 - R0) * pow(1.0 - HdotV, 5.0)));
}

//r + (N - rN) = 0
//r + N - rN = 0
// r - r/N == 1

// --- VERIFIED ---

//1 on failure OBSTRUCTED
Bool    check_obstruction(t_light *l, vec3 p)
{
        ray check;
        double max_t = INFINITY;

        if (l->is_dir)
        {
            check.orig = v_add(p, v_scal(l->dir, -1000));
            check.dir = l->dir;
        }
        else //Point light
        {
            check.orig = l->pos;
            check.dir = v_norm(from_to(l->pos, p));
            max_t = vec_dist(l->pos, p) - 0.001;
        }
        return check_hit(&check, (interval){0.0, max_t});
}

void   set_IBL(shader_end *se, hit_record *rec, ray *ray_in,
    double metalic, double roughness, color albedo, double R0, double NdotV, double specularity, double transmission)
{
    se->diffuse_color = v_scal(albedo, (1.0 - metalic));
    
    vec3 F = fresnel_shlick(NdotV, R0);
    vec3 kD =  v_sub(v_3(1.0), F);

    color spec_tint = lerp(rec->mat.specular_tint, WHITE, v_len(albedo)>0?v_norm(albedo):WHITE);
    spec_tint = lerp(F.x, spec_tint, WHITE);
    se->specular_color = v_add(v_mult(F, spec_tint), v_scal(albedo, metalic));
    if (v.background_color != NULL) return;

    vec3 L = reflect_safe(ray_in->dir, v_add(rec->normal, v_scal(random_unit_vector(), roughness)), rec->old_normal);
    t_light IBL; IBL.is_dir = 1; IBL.dir = v_scal(L, -1);
    vec3 uv = v_scal(v_add(L, v_3(1)), 0.5);
    color irradiance_sample = evaluate(&v.irradiance_map, uv.x, uv.y);

    if (check_obstruction(&IBL, rec->p) || (metalic == 0.0 && specularity == 0.0 && transmission == 0.0))
        se->specular_light = BLACK;
    else
        se->specular_light = irradiance_sample;

    //DIFFUSE

    //dl blurred from irradiance.. idk
    //dl/dc=0.0 if FULL metalic/transmission
    color blurry_irradiance_sample = evaluate(&v.blurry_irradiance_map, uv.x, uv.y);
    se->diffuse_light = v_scal(v_mult(kD, blurry_irradiance_sample), (1.0 - transmission));
}

shader_end CalcTotalPBRLighting(hit_record *rec, ray *ray_in)
{  
    double  metalic     = evaluate_bw(&rec->mat.metalic, rec->u, rec->v);
    double  roughness   = evaluate_bw(&rec->mat.roughness, rec->u, rec->v);
    double  specularity = evaluate_bw(&rec->mat.specular, rec->u, rec->v);
    double  transmission = rec->mat.transmission;
    vec3    albedo      = evaluate(&rec->mat.base_color, rec->u, rec->v);
    double  R0 = specularity * 0.16;

    vec3 N = rec->normal;
    vec3 V = v_scal(ray_in->dir, -1);
    double NdotV = fmax(v_dot(N, V), 0.0000001);

    shader_end  se = (shader_end){};
    set_IBL(&se, rec, ray_in, metalic, roughness, albedo, R0, NdotV, specularity, transmission);

    for (int i=0; i<v.light_count; i++)
    {
        t_light *l = &v.lights[i];
        if (check_obstruction(l, rec->p)) continue;

        vec3 L = l->is_dir ? v_scal(l->dir, -1) : v_norm(from_to(rec->p, l->pos));
        vec3 H = v_norm(v_add(V, L));
        double distance = v_len(v_sub(l->pos, rec->p));
        double attenuation = l->is_dir?1.0  :  1.0 / (distance * distance);
        vec3 radiance = v_scal(l->col, attenuation * l->intensity);

        //Cook-Torrence BRDF
        double NdotL = fmax(v_dot(N, L), 0.0000001);
        double HdotV = fmax(v_dot(H, V), 0.0);
        double NdotH = fmax(v_dot(N, H), 0.0);

        double D = distribution_ggx(NdotH, roughness);
        double G = geometry_smith(NdotV, NdotL, roughness);
        vec3   F = fresnel_shlick(HdotV, R0);

        vec3 specular = v_scal(F, G * D); // D * G * F
        specular = v_scal(specular, 1.0 / (4.0 * NdotV * NdotL));

        //conservation of energy
        vec3 kd = v_sub(v_3(1.0), F);

        //combine
        se.diffuse_light = v_add(se.diffuse_light, 
                        v_mult(kd, v_scal(radiance, NdotL)));
        se.specular_color = v_add(se.specular_color, specular);
        se.specular_light = v_add(se.specular_light, v_scal(radiance, NdotL));
    }

    return se;
}
