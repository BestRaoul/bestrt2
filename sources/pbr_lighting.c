#include "fractol.h"

vec3 schlickFresnel(float vDotH, Bool isMetal, vec3 base_color)
{
    vec3 F0 = v3(0.04);

    if (isMetal) {
        F0 = base_color;
    }

    vec3 ret = v_add(F0, 
               v_scal(v_sub(v_3(1), F0), pow(clamp_(1.0 - vDotH), 5)));

    return ret;
}


float geomSmith(float dp, double roughness)
{
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    float denom = dp * (1 - k) + k;
    return dp / denom;
}


float ggxDistribution(float nDotH, double roughness)
{
    float alpha2 = roughness * roughness * roughness * roughness;
    float d = nDotH * nDotH * (alpha2 - 1) + 1;
    float ggxdistrib = alpha2 / (MYPI * d * d);
    return ggxdistrib;
}


vec3 CalcPBRLighting(t_light *light, hit_record *rec, ray *ray_in)
{
    double  metalic = evaluate_bw(&rec->mat.metalic, rec->u, rec->v);
    double  rougness = evaluate_bw(&rec->mat.roughness, rec->u, rec->v);
    vec3  base_color = evaluate(&rec->mat.base_color, rec->u, rec->v);

    vec3 LightIntensity = v_scal(light->col, light->intensity);

    vec3 l = v3(0.0);

    if (light->is_dir) {
        l = v_scal(light->pos_dir, -1);
    } else {
        l = v_sub(light->pos_dir, rec->p);
        float LightToPixelDist = v_len(l);
        l = v_norm(l);
        LightIntensity = v_scal(LightIntensity, 1.0 / (LightToPixelDist * LightToPixelDist));
    }

    vec3 n = rec->normal;
    vec3 v = ray_in->dir;
    vec3 h = v_norm(v_add(v, l));

    float nDotH = fmax(v_dot(n, h), 0.0);
    float vDotH = fmax(v_dot(v, h), 0.0);
    float nDotL = fmax(v_dot(n, l), 0.0);
    float nDotV = fmax(v_dot(n, v), 0.0);

    vec3 F = schlickFresnel(vDotH, metalic, base_color);

    vec3 kS = F;
    vec3 kD = v_sub(v_3(1.0), kS);

    vec3 SpecBRDF_nom  = v_scal(F,
                         ggxDistribution(nDotH, rougness) *
                         geomSmith(nDotL, rougness) *
                         geomSmith(nDotV, rougness));

    float SpecBRDF_denom = 4.0 * nDotV * nDotL + 0.0001;

    vec3 SpecBRDF = v_scal(SpecBRDF_nom, 1.0 / SpecBRDF_denom);

    vec3 fLambert = v3(0.0);

    if (!metalic) {
        fLambert = base_color;
    }

    vec3 DiffuseBRDF = v_scal(v_mult(kD, fLambert), 1.0 / MYPI);

    vec3 FinalColor = v_mult( v_add(DiffuseBRDF, SpecBRDF),
                              v_scal(LightIntensity, nDotL));

    return FinalColor;
}


/*
vec3 CalcPBRDirectionalLight(vec3 Normal)
{
    return CalcPBRLighting(gDirectionalLight.Base, gDirectionalLight.Direction, true, Normal);
}


vec3 CalcPBRPointLight(PointLight l, vec3 Normal)
{
    return CalcPBRLighting(l.Base, l.LocalPos, false, Normal);
}
*/

//0 means no obstruction
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

//list of all point lights (think about how to sample emittingobjects)
//list of directional lights
//
//normal0 ->n
//texCoord0 ->UV
//localPos0 ->p
//
//cameraLocalPos
//gMaterial
vec3 CalcTotalPBRLighting(hit_record *rec, ray *ray_in)
{
    vec3 TotalLight = v3();
    
    for (int i = 0 ; i<v.light_count ;i++) {
        t_light *l = &(v.lights[i]);

        //check if anything block
        if (check_obstruction(l, rec->p))
            continue;
        
        //add light from source
        TotalLight = v_add(TotalLight, CalcPBRLighting(l, rec, ray_in));
    }

    // HDR tone mapping
    //TotalLight = TotalLight / (TotalLight + vec3(1.0));
    TotalLight = v_div(TotalLight, v_add(TotalLight, v3(1,1,1)));


    // Gamma correction
    vec3 FinalLight = TotalLight;
    //vec4 FinalLight = vec4(pow(TotalLight, vec3(1.0/2.2)), 1.0);

    return FinalLight;
}