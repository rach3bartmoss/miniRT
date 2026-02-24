/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_caps.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 23:02:28 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 21:24:59 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

/// @brief 
/// @param cap_ctx 
/// @param cylinder 
/// @param flag Set 0 for bottom cap and 1 for top cap
static void	init_cap_ctx(t_cy_cap *cap_ctx, t_cy_ctx *cylinder, int flag)
{
	if (flag == 0)
	{
		copy_vectors(cap_ctx->center, cylinder->base_center);
		scale(cap_ctx->cap_normal, cylinder->normal, -1.0f);
	}
	else if (flag == 1)
	{
		copy_vectors(cap_ctx->center, cylinder->top_center);
		copy_vectors(cap_ctx->cap_normal, cylinder->normal);
	}
	cap_ctx->denom = dot(cylinder->d, cap_ctx->cap_normal);
	cap_ctx->tcap = -1.0f;
	cap_ctx->radius = cylinder->curr_cy->cy_diameter / 2;
}

/// @brief 1) find the ray-plane intersect for the plane containing the cap
/// @brief 2) then check if the intersection point is within the cap's radius
/// @brief 3) IF |P - C| * |P - C| < r² INTERSECTION HAPPENS
/// @brief 4) Where P = intersection point
/// @brief 5) C = cap center point
/// @brief 6) r = radius of cap
/// @brief 7) return on caller cap_ctx->tcap != -1.0f (true)
/// @param cap_ctx Cylinder cap context variables, use for calcs
/// @param cy_ctx Current cylinder context varaibles
/// @param flag Flag to differ from camera-ray and shadow-ray usage
static void	calc_cap_helper(t_cy_cap *cap_ctx, t_cy_ctx *cy_ctx, int flag)
{
	double	origin_minus_center[3];
	double	tmp[3];

	sub(origin_minus_center, cap_ctx->center, cy_ctx->origin);
	cap_ctx->tcap = dot(origin_minus_center, cap_ctx->cap_normal);
	cap_ctx->tcap /= cap_ctx->denom;
	if (cap_ctx->tcap > 0.0 && cap_ctx->tcap < cy_ctx->rec->t)
	{
		scale(tmp, cy_ctx->d, (double)cap_ctx->tcap);
		add(cap_ctx->hit_cap_p, cy_ctx->origin, tmp);
		sub(tmp, cap_ctx->hit_cap_p, cap_ctx->center);
		if (dot(tmp, tmp) <= (cap_ctx->radius * cap_ctx->radius))
		{
			if (flag == 1)
			{
				cy_ctx->rec->t = cap_ctx->tcap;
				cy_ctx->rec->hit = 1;
				cy_ctx->rec->object_idx = cy_ctx->i;
				cy_ctx->rec->obj_scene_idx = cy_ctx->s;
				copy_vectors(cy_ctx->rec->hit_point, cap_ctx->hit_cap_p);
				copy_vectors(cy_ctx->rec->normal, cap_ctx->cap_normal);
				copy_int_vectors(cy_ctx->rec->color, cy_ctx->curr_cy->cy_rgb);
			}
		}
	}
}

double	cylinder_bottom_cap(t_cy_ctx *cy_ctx, int flag)
{
	t_cy_cap	cap_ctx;

	init_cap_ctx(&cap_ctx, cy_ctx, 0);
	if (fabs(cap_ctx.denom) > 1e-6)
	{
		calc_cap_helper(&cap_ctx, cy_ctx, flag);
	}
	return (cap_ctx.tcap);
}

double	cylinder_top_cap(t_cy_ctx *cy_ctx, int flag)
{
	t_cy_cap	cap_ctx;

	init_cap_ctx(&cap_ctx, cy_ctx, 1);
	if (fabs(cap_ctx.denom) > 1e-6)
	{
		calc_cap_helper(&cap_ctx, cy_ctx, flag);
	}
	return (cap_ctx.tcap);
}
