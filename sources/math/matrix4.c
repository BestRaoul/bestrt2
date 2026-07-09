
#include "fractol.h"

//multiply mattrix with matrix
s_m4	mm(const m4x4 a, const m4x4 b)
{
	s_m4	c;
	int		i;
	int		j;
	int		k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			c.mat[i][j] = 0;
			k = 0;
			while (k < 4)
			{
				c.mat[i][j] += a[i][k] * b[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
	return (c);
}

//multiply matrix with point
vec3	mult_point_matrix(const vec3 in, const m4x4 M)
{
	vec3	out;
	PFPN	w;

	w = in.x * M[0][3] + in.y * M[1][3] + in.z * M[2][3] + M[3][3];
	out.x = in.x * M[0][0] + in.y * M[1][0] + in.z * M[2][0] + M[3][0];
	out.y = in.x * M[0][1] + in.y * M[1][1] + in.z * M[2][1] + M[3][1];
	out.z = in.x * M[0][2] + in.y * M[1][2] + in.z * M[2][2] + M[3][2];
	if (w != 1)
	{
		out.x /= w;
		out.y /= w;
		out.z /= w;
	}
	return (out);
}

// // normalize if w is different than 1
// //(convert from homogeneous to Cartesian coordinates)
// //unused
void	multiply_matrix_vector(const m4x4 mat, const vec3 in, vec3 *out)
{
	PFPN	w;

	out->x = mat[0][0] * in.x + mat[0][1] * in.y + mat[0][2] * in.z
		+ /* 1  this is literally the best feature there is */ mat[0][3];
	out->y = mat[1][0] * in.x + mat[1][1] * in.y + mat[1][2] * in.z
		+ /* 1 */ mat[1][3];
	out->z = mat[2][0] * in.x + mat[2][1] * in.y + mat[2][2] * in.z
		+ /* 1 */ mat[2][3];
	w = mat[3][0] * in.x + mat[3][1] * in.y + mat[3][2] * in.z
		+ /* 1 */ mat[3][3];
	if (w != 1)
	{
		out->x /= w;
		out->y /= w;
		out->z /= w;
	}
}

void	print_mx4(m4x4 m)
{
	int	i;

	printf("MATRIX 4X4 : {\n");
	i = 0;
	while (i < 4)
	{
		printf("% 4.4f, % 4.4f, % 4.4f, % 4.4f;", m[i][0], m[i][1], m[i][2],
			m[i][3]);
		if (i != 3)
			printf("\n");
		i++;
	}
	printf("}\n");
}

const m4x4	g_identity = {
{1, 0, 0, 0},
{0, 1, 0, 0},
{0, 0, 1, 0},
{0, 0, 0, 1}};

void	set_m4(m4x4 m, const m4x4 s)
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

void	set_identity(m4x4 m)
{
	set_m4(m, g_identity);
}

// Augmented matrix [in | I], where I is the 4x4 identity matrix
static void	init(PFPN aug[4][8], const m4x4 in)
{
	int	i;
	int	j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			aug[i][j] = in[i][j];
			aug[i][j + 4] = (i == j);
			j++;
		}
		i++;
	}
}

// Perform row operations to get the upper triangular form
static void	upper_triangular_form(PFPN aug[4][8])
{
	PFPN	ratio;
	int		i;
	int		j;
	int		k;

	i = 0;
	while (i < 4)
	{
		j = i + 1;
		while (j < 4)
		{
			ratio = aug[j][i] / aug[i][i];
			k = 0;
			while (k < 8)
			{
				aug[j][k] -= ratio * aug[i][k];
				k++;
			}
			j++;
		}
		i++;
	}
}

static void	back_substition(PFPN aug[4][8])
{
	PFPN	ratio;
	int		i;
	int		j;
	int		k;

	i = 3;
	while (i > 0)
	{
		j = i - 1;
		while (j >= 0)
		{
			ratio = aug[j][i] / aug[i][i];
			k = 0;
			while (k < 8)
			{
				aug[j][k] -= ratio * aug[i][k];
				k++;
			}
			j--;
		}
		i--;
	}
}

static void	normalize(PFPN aug[4][8])
{
	PFPN	divisor;
	int		i;
	int		j;
	int		k;

	i = 0;
	while (i < 4)
	{
		divisor = aug[i][i];
		j = 0;
		while (j < 8)
		{
			aug[i][j] /= divisor;
			j++;
		}
		i++;
	}
}

void	inverse(const m4x4 in, m4x4 out)
{
	PFPN	aug[4][8];
	int		i;
	int		j;

	init(aug, in);
	upper_triangular_form(aug);
	back_substition(aug);
	normalize(aug);
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			out[i][j] = aug[i][j + 4];
			j++;
		}
		i++;
	}
}

static inline void	set_translation(m4x4 m, const tfm *transform)
{
	set_identity(m);
	m[0][3] = transform->pos.x;
	m[1][3] = transform->pos.y;
	m[2][3] = transform->pos.z;
}

// cheating to avoid GIMBAL LOCK flattening,
// if you figure out how not to flatten with given angles feel free to correct
static inline void	set_rotation(m4x4 m, const tfm *transform)
{
	vec3	_;
	vec3	coss;
	vec3	sinn;

	_ = transform->rot;
	if (((fabs(fmod(fabs(_.x), MYPI) - MYPI / 2) < MYPI / 100)
			|| (fabs(fmod(fabs(_.z), MYPI) - MYPI / 2) < MYPI / 100))
		&& fabs(fmod(fabs(_.y), MYPI) - MYPI / 2) < MYPI / 100)
		_.y = _.y - signd(_.y) * MYPI / 100;
	coss.x = cos(_.x);
	sinn.x = sin(_.x);
	coss.y = cos(_.y);
	sinn.y = sin(_.y);
	coss.z = cos(_.z);
	sinn.z = sin(_.z);
	set_identity(m);
	m[0][0] = coss.y * coss.z;
	m[0][1] = sinn.x * sinn.y * coss.z - coss.x * sinn.z;
	m[0][2] = coss.x * sinn.y * coss.z + sinn.x * sinn.z;
	m[1][0] = coss.y * sinn.z;
	m[1][1] = sinn.x * sinn.y * sinn.z + coss.x * coss.z;
	m[1][2] = coss.x * sinn.y * sinn.z - sinn.x * coss.z;
	m[2][0] = -sinn.y;
	m[2][1] = sinn.x * coss.y;
	m[2][2] = coss.x * coss.y;
}

static inline void	set_scale(m4x4 m, const tfm *transform)
{
	set_identity(m);
	m[0][0] = transform->scale.x;
	m[1][1] = transform->scale.y;
	m[2][2] = transform->scale.z;
}

void	set_transform_matrix(const tfm *transform, m4x4 m_fwd, m4x4 m_bck)
{
	m4x4	translation_matrix;
	m4x4	rotation_matrix;
	m4x4	scale_matrix;
	s_m4	s_fwd;

	set_translation(translation_matrix, transform);
	set_rotation(rotation_matrix, transform);
	set_scale(scale_matrix, transform);
	s_fwd = mm(translation_matrix, mm(rotation_matrix, scale_matrix).mat);
	set_m4(m_fwd, s_fwd.mat);
	inverse(m_fwd, m_bck);
}

/*
m4x4				rotation_matrix_x   = {};
m4x4				rotation_matrix_y   = {};
m4x4				rotation_matrix_z   = {};

set_identity(rotation_matrix_x);
set_identity(rotation_matrix_y);
set_identity(rotation_matrix_z);

//Rotations
rotation_matrix_z[0][0] = cos(transform->rot.z);
rotation_matrix_z[0][1] = -sin(transform->rot.z);
rotation_matrix_z[1][0] = sin(transform->rot.z);
rotation_matrix_z[1][1] = cos(transform->rot.z);

rotation_matrix_y[0][0] = cos(transform->rot.y);
rotation_matrix_y[0][2] = sin(transform->rot.y);
rotation_matrix_y[2][0] = -sin(transform->rot.y);
rotation_matrix_y[2][2] = cos(transform->rot.y);

rotation_matrix_x[1][1] = cos(transform->rot.x);
rotation_matrix_x[1][2] = -sin(transform->rot.x);
rotation_matrix_x[2][1] = sin(transform->rot.x);
rotation_matrix_x[2][2] = cos(transform->rot.x);

s_m4				temp = mm(rotation_matrix_y, mm(rotation_matrix_z,
							rotation_matrix_x).mat);
m4x4				rotation_matrix;
set_m4(rotation_matrix, temp.mat);
*/