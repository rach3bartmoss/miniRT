/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 20:16:18 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 21:24:59 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	copy_vectors(double out[3], double in[3])
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

void	mult(double target_xyz[3], double a[3], double b[3])
{
	target_xyz[0] = a[0] * b[0];
	target_xyz[1] = a[1] * b[1];
	target_xyz[2] = a[2] * b[2];
}

void	normalize_colors(double rgb[3])
{
	rgb[0] /= 255.0f;
	rgb[1] /= 255.0f;
	rgb[2] /= 255.0f;
}

void	normalize_target_colors(double target_xyz[3], int rgb[3])
{
	target_xyz[0] = rgb[0] / 255.0f;
	target_xyz[1] = rgb[1] / 255.0f;
	target_xyz[2] = rgb[2] / 255.0f;
}
