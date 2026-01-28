/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_checkerboard_helper.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 23:11:44 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/28 23:16:23 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief Multiplies a 4x4 matrix by a point (3D vector)
/// @brief Needed for UV Mapping from a 2D texture into a 3D Cylinder (tube)
/// @brief Basically will perform calculations to the hitpoint behave as the
/// @brief center of the cylinder
/// @param result 
/// @param m 4x4 Matrix 
/// @param p 3D-Vector point
void	apply_matrix(float result[3], float m[4][4], float p[3])
{
	float	w;

	result[0] = p[0] * m[0][0] + p[1] * m[0][1] + p[2] * m[0][2] + m[0][3];
	result[1] = p[0] * m[1][0] + p[1] * m[1][1] + p[2] * m[1][2] + m[1][3];
	result[2] = p[0] * m[2][0] + p[1] * m[2][1] + p[2] * m[2][2] + m[2][3];
	w = p[0] * m[3][0] + p[1] * m[3][1] + p[2] * m[3][2] + m[3][3];
	if (w != 0.0f && w != 1.0f)
	{
		result[0] /= w;
		result[1] /= w;
		result[2] /= w;
	}
}

void	fill_inv_matrix_helper(float m[4][4], float axis[3], float right[3],
			float forward[3])
{
	m[0][0] = right[0];
	m[0][1] = right[1];
	m[0][2] = right[2];
	m[1][0] = axis[0];
	m[1][1] = axis[1];
	m[1][2] = axis[2];
	m[2][0] = forward[0];
	m[2][1] = forward[1];
	m[2][2] = forward[2];
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

/// @brief Invert a 4x4 matrix based on the cylinder object coordinates,
/// @brief translate a "world" hit point position into a "object" position
/// @brief Sum: *Calculate the inverse matrix for this cylinder*
/// @param m 
/// @param cy 
void	fill_inv_matrix(float m[4][4], t_cylinder *cy)
{
	float	axis[3];
	float	right[3];
	float	forward[3];
	float	temp[3];
	float	neg_pos[3];

	set_vec_float_values(temp, 0.0f, 0.0f, 1.0f);
	normalize(cy->cy_vector_xyz, axis);
	if (fabsf(axis[2]) > 0.9f)
	{
		temp[0] = 1.0f;
		temp[2] = 0.0f;
	}
	cross(axis, temp, right);
	normalize(right, right);
	cross(right, axis, forward);
	normalize(forward, forward);
	fill_inv_matrix_helper(m, axis, right, forward);
	set_vec_float_values(neg_pos, -cy->cy_xyz[0], -cy->cy_xyz[1],
		-cy->cy_xyz[2]);
	m[0][3] = (float)dot(right, neg_pos);
	m[1][3] = (float)dot(axis, neg_pos);
	m[2][3] = (float)dot(forward, neg_pos);
}
