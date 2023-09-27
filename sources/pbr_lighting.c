#include "fractol.h"
/*
vec3 schlickFresnel(float vDotH)
{
    vec3 F0 = vec3(0.04);

    if (gPBRmaterial.IsMetal) {
        F0 = gPBRmaterial.Color;
    }

    vec3 ret = F0 + (1 - F0) * pow(clamp(1.0 - vDotH, 0.0, 1.0), 5);

    return ret;
}


float geomSmith(float dp)
{
    float k = (gPBRmaterial.Roughness + 1.0) * (gPBRmaterial.Roughness + 1.0) / 8.0;
    float denom = dp * (1 - k) + k;
    return dp / denom;
}


float ggxDistribution(float nDotH)
{
    float alpha2 = gPBRmaterial.Roughness * gPBRmaterial.Roughness * gPBRmaterial.Roughness * gPBRmaterial.Roughness;
    float d = nDotH * nDotH * (alpha2 - 1) + 1;
    float ggxdistrib = alpha2 / (PI * d * d);
    return ggxdistrib;
}


vec3 CalcPBRLighting(BaseLight Light, vec3 PosDir, bool IsDirLight, vec3 Normal)
{
    vec3 LightIntensity = Light.Color * Light.DiffuseIntensity;

    vec3 l = vec3(0.0);

    if (IsDirLight) {
        l = -PosDir.xyz;
    } else {
        l = PosDir - LocalPos0;
        float LightToPixelDist = length(l);
        l = normalize(l);
        LightIntensity /= (LightToPixelDist * LightToPixelDist);
    }

    vec3 n = Normal;
    vec3 v = normalize(gCameraLocalPos - LocalPos0);
    vec3 h = normalize(v + l);

    float nDotH = max(dot(n, h), 0.0);
    float vDotH = max(dot(v, h), 0.0);
    float nDotL = max(dot(n, l), 0.0);
    float nDotV = max(dot(n, v), 0.0);

    vec3 F = schlickFresnel(vDotH);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;

    vec3 SpecBRDF_nom  = ggxDistribution(nDotH) *
                         F *
                         geomSmith(nDotL) *
                         geomSmith(nDotV);

    float SpecBRDF_denom = 4.0 * nDotV * nDotL + 0.0001;

    vec3 SpecBRDF = SpecBRDF_nom / SpecBRDF_denom;

    vec3 fLambert = vec3(0.0);

    if (!gPBRmaterial.IsMetal) {
        fLambert = gPBRmaterial.Color;
    }

    vec3 DiffuseBRDF = kD * fLambert / PI;

    vec3 FinalColor = (DiffuseBRDF + SpecBRDF) * LightIntensity * nDotL;

    return FinalColor;
}


vec3 CalcPBRDirectionalLight(vec3 Normal)
{
    return CalcPBRLighting(gDirectionalLight.Base, gDirectionalLight.Direction, true, Normal);
}


vec3 CalcPBRPointLight(PointLight l, vec3 Normal)
{
    return CalcPBRLighting(l.Base, l.LocalPos, false, Normal);
}


//list of all point lights (think about how to sample emittingobjects)
//list of directional lights
//
//normal0 ->n
//texCoord0 ->UV
//localPos0 ->p which what>?>?
vec4 CalcTotalPBRLighting()
{
    vec3 Normal = normalize(Normal0);

    vec3 TotalLight = CalcPBRDirectionalLight(Normal);

    for (int i = 0 ;i < gNumPointLights ;i++) {
        TotalLight += CalcPBRPointLight(gPointLights[i], Normal);
    }

    // HDR tone mapping
    TotalLight = TotalLight / (TotalLight + vec3(1.0));

    // Gamma correction
    vec4 FinalLight = vec4(pow(TotalLight, vec3(1.0/2.2)), 1.0);

    return FinalLight;
}
*/