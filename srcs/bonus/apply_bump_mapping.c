/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_bump_mapping.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 12:40:47 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 01:26:36 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	ft_clamp(float value, float min, float max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

void	build_tangent_space(float normal[3],
							float tangent[3],
							float bitangent[3])
{
	float	up[3];

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
	cross(up, normal, tangent);
	normalize(tangent, tangent);
	cross(normal, tangent, bitangent);
	normalize(bitangent, bitangent);
}

float	sample_height(t_texture *tex, float u, float v)
{
	t_bump_height_ctx	ctx;

	u = u - floorf(u);
	v = v - floorf(v);
	ctx.x = (int)(u * (tex->width - 1));
	ctx.y = (int)(v * (tex->height - 1));
	ctx.offset = ctx.y * tex->line_len + ctx.x * (tex->bpp / 8);
	ctx.pixel = *(unsigned int *)(tex->buffer + ctx.offset);
	ctx.r = (float)((ctx.pixel >> 16) & 0xFF);
	ctx.g = (float)((ctx.pixel >> 8) & 0xFF);
	ctx.b = (float)(ctx.pixel & 0xFF);
	return ((ctx.r + ctx.g + ctx.b) / 3.0f / 255.0f);
}

void	apply_sphere_bump(t_hit *hit, t_texture *bump)
{
	t_sp_bump_ctx	ctx;
	int				i;

	i = 0;
	ctx.force = BUMP_STRENGTH;
	copy_vectors(ctx.n, hit->shading_normal);
	ctx.uv[0] = 0.5f + atan2f(ctx.n[2], ctx.n[0]) / (2.0f * M_PI);
	ctx.uv[1] = 0.5f - asinf(ctx.n[1]) / M_PI;
	ctx.du = 1.0f / bump->width;
	ctx.dv = 1.0f / bump->height;
	ctx.h = sample_height(bump, ctx.uv[0], ctx.uv[1]);
	ctx.hu = sample_height(bump, ctx.uv[0] + ctx.du, ctx.uv[1]);
	ctx.hv = sample_height(bump, ctx.uv[0], ctx.uv[1] + ctx.dv);
	ctx.d_u = ft_clamp(ctx.hu - ctx.h, -0.2f, 0.2f);
	ctx.d_v = ft_clamp(ctx.hv - ctx.h, -0.2f, 0.2f);
	build_tangent_space(hit->shading_normal, ctx.tangent, ctx.bitangent);
	while (i < 3)
	{
		ctx.new_normal[i] = hit->shading_normal[i] + ctx.force * ctx.d_u
			* ctx.tangent[i] + ctx.force * ctx.d_v * ctx.bitangent[i];
		i++;
	}
	normalize(ctx.new_normal, ctx.new_normal);
	copy_vectors(hit->shading_normal, ctx.new_normal);
}
