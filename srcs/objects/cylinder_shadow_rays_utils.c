/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_shadow_rays_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 21:26:38 by dopereir          #+#    #+#             */
/*   Updated: 2025/11/16 18:26:30 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief Calculates the cylinder bottom area intersection in the shadow-ray
/// @brief casting context.
/// @brief 1) P(t) = Origin + t * Direction ∈ (Origin < t < t_max)
/// @brief 2) Need to fetch the cylinder axis vector (Normalized)
/// @brief 3) Denom = Direction * nbot
/// @brief 4) hit_cap = ( (center - Origin) * nbot ) / denom
/// @brief 5) only accept hit_cap if 1e-4 < hit_cap < t_max
/// @param cy_ctx The current cylinder object and useful parameters
/// @param t_max The distance between the ray-origin point and the light
/// @return If the intersection lays between t_max boundaries return hit_cap.
double	cylinder_bottom_sr(t_cy_ctx *cy_ctx, float t_max)
{
	t_cy_cap	sray_ctx;
	float		tmp[3];
	float		center_minus_origin[3];

	sray_ctx.r = cy_ctx->curr_cy->cy_diameter / 2;
	scale(sray_ctx.cap_normal, cy_ctx->normal, -1.0f);
	sray_ctx.hit_cap = -1.0f;
	sray_ctx.denom = dot(cy_ctx->d, sray_ctx.cap_normal);
	if (fabs(sray_ctx.denom) < 1e-6f)
		return (-1.0f);
	sub(center_minus_origin, cy_ctx->base_center, cy_ctx->origin);
	sray_ctx.hit_cap = dot(center_minus_origin, sray_ctx.cap_normal)
		/ sray_ctx.denom;
	if (sray_ctx.hit_cap <= 1e-4f || sray_ctx.hit_cap >= t_max)
		return (-1.0f);
	scale(tmp, cy_ctx->d, (float)sray_ctx.hit_cap);
	add(sray_ctx.hit_cap_point, cy_ctx->origin, tmp);
	sub(tmp, sray_ctx.hit_cap_point, cy_ctx->base_center);
	if (dot(tmp, tmp) <= (sray_ctx.r * sray_ctx.r))
		return (sray_ctx.hit_cap);
	return (-1.0f);
}

/// @brief Same logic as cylinder_bottom_sr but for top cap
/// @param cy_ctx 
/// @param t_max 
/// @return If the intersection lays between t_max boundaries return hit_cap.
double	cylinder_top_sr(t_cy_ctx *cy_ctx, float t_max)
{
	t_cy_cap	sray_ctx;
	float		tmp[3];
	float		center_minus_origin[3];

	sray_ctx.hit_cap = -1.0f;
	sray_ctx.denom = dot(cy_ctx->d, cy_ctx->normal);
	if (fabs(sray_ctx.denom) < 1e-6f)
		return (-1.0f);
	sub(center_minus_origin, cy_ctx->top_center, cy_ctx->origin);
	sray_ctx.hit_cap = dot(center_minus_origin, cy_ctx->normal)
		/ sray_ctx.denom;
	if (sray_ctx.hit_cap <= 1e-4f || sray_ctx.hit_cap >= t_max)
		return (-1.0f);
	scale(tmp, cy_ctx->d, (float)sray_ctx.hit_cap);
	add(sray_ctx.hit_cap_point, cy_ctx->origin, tmp);
	sub(tmp, sray_ctx.hit_cap_point, cy_ctx->top_center);
	if (dot(tmp, tmp) <= (cy_ctx->radius * cy_ctx->radius))
		return (sray_ctx.hit_cap);
	else
		return (-1.0f);
}

void	prep_sr_cy_intersect(t_cy_ctx *cy_ctx, float *sr_origin, float *sr_dir)
{
	copy_vectors(cy_ctx->normal, cy_ctx->curr_cy->cy_vector_xyz);
	copy_vectors(cy_ctx->d, sr_dir);
	normalize(cy_ctx->normal, cy_ctx->normal);
	copy_vectors(cy_ctx->origin, sr_origin);
	cy_ctx->half_height = cy_ctx->curr_cy->cy_height / 2.0f;
	cy_ctx->radius = cy_ctx->curr_cy->cy_diameter / 2.0f;
	scale(cy_ctx->ha, cy_ctx->normal, cy_ctx->half_height);
	sub(cy_ctx->base_center, cy_ctx->curr_cy->cy_xyz, cy_ctx->ha);
	add(cy_ctx->top_center, cy_ctx->curr_cy->cy_xyz, cy_ctx->ha);
}

float	comp_finite_height_for_light(float t_side, t_cy_ctx *cy_ctx,
	float t_max)
{
	float	hit_p[3];
	float	scale_td[3];
	float	hitp_minus_center[3];

	if (t_side <= 0.0f || t_side >= t_max)
		return (-1.0f);
	scale(scale_td, cy_ctx->d, (float)t_side);
	add(hit_p, cy_ctx->origin, scale_td);
	sub(hitp_minus_center, hit_p, cy_ctx->curr_cy->cy_xyz);
	cy_ctx->y = dot(hitp_minus_center, cy_ctx->normal);
	if (cy_ctx->y >= -cy_ctx->half_height && cy_ctx->y <= cy_ctx->half_height)
	{
		return (t_side);
	}
	return (-1.0f);
}

float	ray_intersection_cy(float *sr_origin, float *sr_dir, t_cylinder *cy,
	float distance)
{
	t_cy_ctx	cy_ctx;
	float		t_side;
	double		hit;
	double		bot;
	double		top;

	cy_ctx.curr_cy = cy;
	prep_sr_cy_intersect(&cy_ctx, sr_origin, sr_dir);
	calc_v_w(&cy_ctx);
	t_side = solve_t_cylinder(cy_ctx.v, cy_ctx.w, &cy_ctx);
	if (t_side > 0.0f)
	{
		hit = comp_finite_height_for_light(t_side, &cy_ctx, distance);
		if (hit > 0.0f)
			return (hit);
	}
	bot = cylinder_bottom_sr(&cy_ctx, distance);
	if (bot > 0.0f)
		return (bot);
	top = cylinder_top_sr(&cy_ctx, distance);
	if (top > 0.0f)
		return (top);
	return (-1.0f);
}
