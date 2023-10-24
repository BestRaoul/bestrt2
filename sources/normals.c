#include "fractol.h"

vec3    perturb_normal(vec3 normal, vec3 perturbation)
{
    vec3 new_up = v3(0, 0, -1);
    if (normal.z > 0.99 || normal.z < -0.99)
        new_up = v3(1, 0, 0);

    vec3    pV = v_cross(new_up, normal);
    pV = v_norm(pV);
    vec3    pU = v_cross(normal, pV);
    pU = v_norm(pU);

    vec3 output = normal;
    output = v_add(output, v_scal(pU, perturbation.y));
    output = v_add(output, v_scal(pV, -perturbation.x));
    output = v_add(output, v_scal(normal, perturbation.z));

    output = v_norm(output);

    return output;
}
