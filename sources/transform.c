/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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
