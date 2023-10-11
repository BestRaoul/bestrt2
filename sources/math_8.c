#include "fractol.h"

void create_transform_matrix(const transform* t, m4x4 result) {
    float sx = t->scale.x;
    float sy = t->scale.y;
    float sz = t->scale.z;
    
    float cx = cosf(t->rot.x);
    float cy = cosf(t->rot.y);
    float cz = cosf(t->rot.z);
    
    float sxz = sinf(t->rot.x) * sinf(t->rot.z);
    float sxy = sinf(t->rot.x) * sinf(t->rot.y);
    float syz = sinf(t->rot.y) * sinf(t->rot.z);
    
    // Set the transformation matrix elements
    result[0][0] = sx * (cy * cz);
    result[0][1] = sx * (-cy * sz);
    result[0][2] = sx * sy;
    result[0][3] = t->pos.x;
    
    result[1][0] = syz * sx;
    result[1][1] = sx * (cx * cz + sxz);
    result[1][2] = sx * (-cx * sz + sxy);
    result[1][3] = t->pos.y;
    
    result[2][0] = -sxy * sx;
    result[2][1] = sx * (cx * sz + syz);
    result[2][2] = sx * (-cx * cz + sxz);
    result[2][3] = t->pos.z;
    
    result[3][0] = 0.0f;
    result[3][1] = 0.0f;
    result[3][2] = 0.0f;
    result[3][3] = 1.0f;
}

void multiply_matrix_vector(const m4x4 mat, const vec3 in, vec3* out) {
    out->x = mat[0][0] * in.x + mat[0][1] * in.y + mat[0][2] * in.z + mat[0][3];
    out->y = mat[1][0] * in.x + mat[1][1] * in.y + mat[1][2] * in.z + mat[1][3];
    out->z = mat[2][0] * in.x + mat[2][1] * in.y + mat[2][2] * in.z + mat[2][3];
}

//DEPRECATED
void multiply_matrix_vector_2(const m4x4 M, const vec3 in, vec3* out)
{ 
    write(1, "DEPRECATED", 11);
    //out = in * M;
    out->x   = in.x * M[0][0] + in.y * M[1][0] + in.z * M[2][0] + /* in.z = 1 */ M[3][0]; 
    out->y   = in.x * M[0][1] + in.y * M[1][1] + in.z * M[2][1] + /* in.z = 1 */ M[3][1]; 
    out->z   = in.x * M[0][2] + in.y * M[1][2] + in.z * M[2][2] + /* in.z = 1 */ M[3][2]; 
    double w = in.x * M[0][3] + in.y * M[1][3] + in.z * M[2][3] + /* in.z = 1 */ M[3][3]; 
 return;
    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    if (w != 1) { 
        out->x /= w; 
        out->y /= w; 
        out->z /= w; 
    } 
}


void    print_mx4(m4x4 m)
{
    printf("MATRIX 4X4 : {\n");
    for (int i=0; i<4; i++)
    {
        printf("% 4.4f, % 4.4f, % 4.4f, % 4.4f;", m[i][0], m[i][1], m[i][2], m[i][3]);
        if (i!=3) printf("\n");
    }
    printf("}\n");
}