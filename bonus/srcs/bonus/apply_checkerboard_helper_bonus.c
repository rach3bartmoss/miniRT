/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_checkerboard_helper.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 23:11:44 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 21:25:11 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

/// @brief Multiplies a 4x4 matrix by a point (3D vector)
/// @brief Needed for UV Mapping from a 2D texture into a 3D Cylinder (tube)
/// @brief Basically will perform calculations to the hitpoint behave as the
/// @brief center of the cylinder
/// @param result 
/// @param m 4x4 Matrix 
/// @param p 3D-Vector point
void	apply_matrix(double result[3], double m[4][4], double p[3])
{
	double	w;

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

void	fill_inv_matrix_helper(double m[4][4], double axis[3], double right[3],
			double forward[3])
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
void	fill_inv_matrix(double m[4][4], t_cylinder *cy)
{
	double	axis[3];
	double	right[3];
	double	forward[3];
	double	temp[3];
	double	neg_pos[3];

	set_vec_double_values(temp, 0.0f, 0.0f, 1.0f);
	normalize(cy->cy_vector_xyz, axis);
	if (fabs(axis[2]) > 0.9f)
	{
		temp[0] = 1.0f;
		temp[2] = 0.0f;
	}
	cross(axis, temp, right);
	normalize(right, right);
	cross(right, axis, forward);
	normalize(forward, forward);
	fill_inv_matrix_helper(m, axis, right, forward);
	set_vec_double_values(neg_pos, -cy->cy_xyz[0], -cy->cy_xyz[1],
		-cy->cy_xyz[2]);
	m[0][3] = (double)dot(right, neg_pos);
	m[1][3] = (double)dot(axis, neg_pos);
	m[2][3] = (double)dot(forward, neg_pos);
}

void	reverse_checkboard_pattern(t_render_ctx *render, t_scene *scene)
{
	int	type;
	int	idx;

	type = render->rec->object_type;
	idx = render->rec->obj_scene_idx;
	if (type == PLANE && scene->plane[idx]->checkerboard == 0
		&& !scene->plane[idx]->base)
		copy_int_vectors(render->rec->color, scene->plane[idx]->pl_rgb);
	else if (type == SPHERE && scene->sphere[idx]->checkerboard == 0
		&& !scene->sphere[idx]->base)
		copy_int_vectors(render->rec->color, scene->sphere[idx]->sp_rgb);
	else if (type == CYLINDER && scene->cylinder[idx]->checkerboard == 0)
		copy_int_vectors(render->rec->color, scene->cylinder[idx]->cy_rgb);
}
