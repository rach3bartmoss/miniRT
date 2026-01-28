/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 20:02:44 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/28 09:24:26 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	ray_length(float vector[3])
{
	double	xyz;

	xyz = (double)vector[0] * vector[0];
	xyz += (double)vector[1] * vector[1];
	xyz += (double)vector[2] * vector[2];
	return (sqrt(xyz));
}

/// @brief Works like a procedural math API, avoids heap allocations
/// @param a_xyz the first float array
/// @param b_xyz the second float array
/// @param target_xyz the resulting cross product of a_xyz and b_xyz;
/// @return 
int	cross(float *a_xyz, float *b_xyz, float *target_xyz)
{
	target_xyz[0] = (a_xyz[1] * b_xyz[2]) - (a_xyz[2] * b_xyz[1]);
	target_xyz[1] = (a_xyz[2] * b_xyz[0]) - (a_xyz[0] * b_xyz[2]);
	target_xyz[2] = (a_xyz[0] * b_xyz[1]) - (a_xyz[1] * b_xyz[0]);
	return (1);
}

void	set_vec_int_values(int vec[3], int va, int vb, int vc)
{
	vec[0] = va;
	vec[1] = vb;
	vec[2] = vc;
}

void	set_vec_float_values(float vec[3], float va, float vb, float vc)
{
	vec[0] = va;
	vec[1] = vb;
	vec[2] = vc;
}
