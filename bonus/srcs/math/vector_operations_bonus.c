/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 20:03:08 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 21:24:59 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

/// @brief Returns a scalar value of the dot product between two 3D vectors
/// @param a 3D-Vector A
/// @param b 3D-Vector B
/// @return a doubleing-point result
double	dot(double a[3], double b[3])
{
	double	res;

	res = 0.0f;
	res += a[0] * b[0];
	res += a[1] * b[1];
	res += a[2] * b[2];
	return (res);
}

/// @brief Subtract (a - b) vectors and store in target.
/// @param target Pointer to target vector
/// @param a Pointer to A vector
/// @param b Pointer to B vector
void	sub(double *target, double *a, double *b)
{
	target[0] = a[0] - b[0];
	target[1] = a[1] - b[1];
	target[2] = a[2] - b[2];
}

/// @brief Add two 3D-Vectors and store in target
/// @param target Pointer to target vector
/// @param a Pointer to A vector
/// @param b Pointer to B vector
void	add(double *target, double *a, double *b)
{
	target[0] = a[0] + b[0];
	target[1] = a[1] + b[1];
	target[2] = a[2] + b[2];
}

/// @brief Multiples a vector by a scalar value
/// @param target To store the result
/// @param a 3-D vector
/// @param scale_factor Scalar value
void	scale(double *target, double *a, double scale_factor)
{
	target[0] = a[0] * scale_factor;
	target[1] = a[1] * scale_factor;
	target[2] = a[2] * scale_factor;
}

/// @brief Normalize a vector orientation
/// @param vector_xyz vector raw value //-0.2, 0.5, 1
/// @param target_xyz vector after normalization, assign to existent variable
/// @return 1 on success, 0 on failure (zero/near-zero) case
int	normalize(double	*vector_xyz, double *target_xyz)
{
	double	len;

	len = sqrtf(dot(vector_xyz, vector_xyz));
	if (len > 1e-6f)
	{
		target_xyz[0] = vector_xyz[0] / len;
		target_xyz[1] = vector_xyz[1] / len;
		target_xyz[2] = vector_xyz[2] / len;
		return (1);
	}
	else
	{
		target_xyz[0] = 0.0f;
		target_xyz[1] = 0.0f;
		target_xyz[2] = 0.0f;
		return (0);
	}
}
