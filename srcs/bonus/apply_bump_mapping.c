/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_bump_mapping.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 12:40:47 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/21 12:57:32 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	build_tangent_space(float normal[3],
							float tangent[3],
							float bitangent[3])
{
	float	up[3];

	// Choose a helper vector that is NOT parallel to normal
	if (fabs(normal[1]) < 0.999f)
	{
		up[0] = 0.0f;
		up[1] = 1.0f;
		up[2] = 0.0f;
	}
	else
	{
		up[0] = 1.0f;
		up[1] = 0.0f;
		up[2] = 0.0f;
	}

	// T = up × normal
	cross(tangent, up, normal);
	normalize(tangent, tangent);

	// B = normal × T
	cross(bitangent, normal, tangent);
	normalize(bitangent, bitangent);
}

float	sample_height(t_texture *tex, float u, float v)
{
	int				x;
	int				y;
	unsigned int	pixel;
	int				offset;
	float			r;
	float			g;
	float			b;

	u = u - floorf(u);
	v = v - floorf(v);

	x = (int)(u * (tex->width - 1));
	y = (int)(v * (tex->height - 1));

	offset = y * tex->line_len + x * (tex->bpp / 8);
	pixel = *(unsigned int *)(tex->buffer + offset);

	r = (float)((pixel >> 16) & 0xFF);
	g = (float)((pixel >> 8) & 0xFF);
	b = (float)(pixel & 0xFF);

	return ((r + g + b) / 3.0f / 255.0f);
}

void	apply_sphere_bump(t_hit *hit, t_texture *bump)
{
	float	n[3];
	float	uv[2];
	float du, dv;
	float h, hu, hv;
	float tangent[3], bitangent[3];
	float new_normal[3];
	float strength = 0.5f;

	copy_vectors(n, hit->normal);
	uv[0] = 0.5f + atan2f(n[2], n[0]) / (2.0f * M_PI);
	uv[1] = 0.5f - asinf(n[1]) / M_PI;

	du = 1.0f / bump->width;
	dv = 1.0f / bump->height;

	h  = sample_height(bump, uv[0], uv[1]);
	hu = sample_height(bump, uv[0] + du, uv[1]);
	hv = sample_height(bump, uv[0], uv[1] + dv);

	float dU = hu - h;
	float dV = hv - h;

	build_tangent_space(hit->normal, tangent, bitangent);

	for (int i = 0; i < 3; i++)
		new_normal[i] = hit->normal[i]
						+ strength * dU * tangent[i]
						+ strength * dV * bitangent[i];

	normalize(new_normal, new_normal);
	copy_vectors(hit->normal, new_normal);
}