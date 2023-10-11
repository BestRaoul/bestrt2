#include "fractol.h"


const m4x4    Identity = 
{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

void    set_m4(m4x4 m, const m4x4 s)
{
    m[0][0] = s[0][0];
    m[0][1] = s[0][1];
    m[0][2] = s[0][2];
    m[0][3] = s[0][3];

    m[1][0] = s[1][0];
    m[1][1] = s[1][1];
    m[1][2] = s[1][2];
    m[1][3] = s[1][3];

    m[2][0] = s[2][0];
    m[2][1] = s[2][1];
    m[2][2] = s[2][2];
    m[2][3] = s[2][3];

    m[3][0] = s[3][0];
    m[3][1] = s[3][1];
    m[3][2] = s[3][2];
    m[3][3] = s[3][3];
}

void    set_identity(m4x4 m)
{
    set_m4(m, Identity);
    /*
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;

    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = 0;

    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
    */
}

/*
vec3	apply(vec3 *p, t_item *item, Bool dirFlag)
{

}

ray	apply_ray(ray *ray_in, t_item *item, Bool dirFlag)
{
	ray output_ray;


}
*/

void inverse(const m4x4 in, m4x4 out) {
    double aug[4][8]; // Augmented matrix [in | I], where I is the 4x4 identity matrix
    int i, j, k;
    
    // Initialize the augmented matrix with the input matrix and identity matrix
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            aug[i][j] = in[i][j];
            aug[i][j + 4] = (i == j) ? 1.0f : 0.0f;
        }
    }

    // Perform row operations to get the upper triangular form
    for (i = 0; i < 4; i++) {
        for (j = i + 1; j < 4; j++) {
            float ratio = aug[j][i] / aug[i][i];
            for (k = 0; k < 8; k++) {
                aug[j][k] -= ratio * aug[i][k];
            }
        }
    }

    // Perform back-substitution to get the identity matrix on the left
    for (i = 3; i > 0; i--) {
        for (j = i - 1; j >= 0; j--) {
            float ratio = aug[j][i] / aug[i][i];
            for (k = 0; k < 8; k++) {
                aug[j][k] -= ratio * aug[i][k];
            }
        }
    }

    // Normalize the rows
    for (i = 0; i < 4; i++) {
        float divisor = aug[i][i];
        for (j = 0; j < 8; j++) {
            aug[i][j] /= divisor;
        }
    }

    // Copy the right side of the augmented matrix to the output matrix
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            out[i][j] = aug[i][j + 4];
        }
    }

    // return 1; // Success
}

void    set_transform_matrix(const transform *t, m4x4 m, m4x4 m_bck)
{
    m4x4    translation_matrix  = {};
    m4x4    rotation_matrix_x   = {};
    m4x4    rotation_matrix_y   = {};
    m4x4    rotation_matrix_z   = {};
    m4x4    scale_matrix        = {};

    //Set all to identity
    set_identity(translation_matrix);
    set_identity(rotation_matrix_x);
    set_identity(rotation_matrix_y);
    set_identity(rotation_matrix_z);
    set_identity(scale_matrix);

    //Translation
    translation_matrix[0][3] = t->pos.x;
    translation_matrix[1][3] = t->pos.y;
    translation_matrix[2][3] = t->pos.z;
    
    //Rotations
	rotation_matrix_z[0][0] = cos(t->rot.z);
	rotation_matrix_z[0][1] = -sin(t->rot.z);
	rotation_matrix_z[1][0] = sin(t->rot.z);
	rotation_matrix_y[0][0] = cos(t->rot.y);
	rotation_matrix_y[0][2] = sin(t->rot.y);
	rotation_matrix_y[2][0] = -sin(t->rot.y);
	rotation_matrix_y[2][2] = cos(t->rot.y);
	
	rotation_matrix_x[1][1] = cos(t->rot.x);
	rotation_matrix_x[1][2] = -sin(t->rot.x);
	rotation_matrix_x[2][1] = sin(t->rot.x);
	rotation_matrix_x[2][2] = cos(t->rot.x);
	
	// And the scale matrix.
	scale_matrix[0][0] = t->scale.x;
	scale_matrix[1][1] = t->scale.y;
	scale_matrix[2][2] = t->scale.z;
	

	s_m4 s_fwd =mm(translation_matrix,
                mm(rotation_matrix_x,
                mm(rotation_matrix_y,
                mm(rotation_matrix_z,
                scale_matrix).mat).mat).mat);
    
    set_m4(m, s_fwd.mat);
    inverse(m, m_bck);

    // Compute the backwards transform.
	// m4x4 bcktfm = fwdtfm;
	//m_bcktfm.Inverse();	

    // item->bck = bcktfm;
}
