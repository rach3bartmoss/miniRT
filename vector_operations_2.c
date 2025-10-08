/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 20:16:18 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/08 23:57:20 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	copy_vectors(float	out[3], float in[3])
{
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
}

void	copy_int_vectors(int out[3], int in[3])
{
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
}

void	mult(float target_xyz[3], float a[3], float b[3])
{
	target_xyz[0] = a[0] * b[0];
	target_xyz[1] = a[1] * b[1];
	target_xyz[2] = a[2] * b[2];
}

void	normalize_target_colors(float target_xyz[3], int rgb[3])
{
	target_xyz[0] = rgb[0] / 255.0f;
	target_xyz[1] = rgb[1] / 255.0f;
	target_xyz[2] = rgb[2] / 255.0f;
}
