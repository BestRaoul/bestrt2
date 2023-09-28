#include "fractol.h"

/*
mat4    Identity = (mat4)
{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

vec3	apply(vec3 *p, t_item *item, Bool dirFlag)
{

}

ray	apply_ray(ray *ray_in, t_item *item, Bool dirFlag)
{
	ray output_ray;


}

void    set_transform(vec3 translation, vec3 rotation, vec3 scale, t_item *item)
{
    mat4    translation_matrix  = (mat4){};
    mat4    rotation_matrix_x   = (mat4){};
    mat4    rotation_matrix_y   = (mat4){};
    mat4    rotation_matrix_z   = (mat4){};
    mat4    scale_matrix        = (mat4){};

    //Set all to identity
    translation_matrix = Identity;
    rotation_matrix_x = Identity;
    rotation_matrix_y = Identity;
    rotation_matrix_z = Identity;
    scale_matrix = Identity;

    //Translation
    translation_matrix.m[0][3] = translation.x;
    translation_matrix.m[1][3] = translation.y;
    translation_matrix.m[2][3] = translation.z;
    
    //Rotations
	rotation_matrix_z[0][0] = cos(rotation.z);
	rotation_matrix_z[0][1] = -sin(rotation.z);
	rotation_matrix_z[1][0] = sin(rotation.z);
	rotation_matrix_z[1][1] = cos(rotation.z);
	
	rotation_matrix_y[0][0] = cos(rotation.y);
	rotation_matrix_y[0][2] = sin(rotation.y);
	rotation_matrix_y[2][0] = -sin(rotation.y);
	rotation_matrix_y[2][2] = cos(rotation.y);
	
	rotation_matrix_x[1][1] = cos(rotation.x);
	rotation_matrix_x[1][2] = -sin(rotation.x);
	rotation_matrix_x[2][1] = sin(rotation.x);
	rotation_matrix_x[2][2] = cos(rotation.x);
	
	// And the scale matrix.
	scale_matrix[0][0] = scale.x;
	scale_matrix[1][1] = scale.y;
	scale_matrix[2][2] = scale.z;
	
	mat4 fwdtfm = mm(translation_matrix,
                mm(rotation_matrix_x,
                mm(rotation_matrix_y,
                mm(rotation_matrix_z,
                mm(scale_matrix)))));
							
	// Compute the backwards transform.
	mat4 bcktfm = fwdtfm;
	//m_bcktfm.Inverse();	

    item->fwd = fwdtfm;
    item->bck = bcktfm;
}
*/
