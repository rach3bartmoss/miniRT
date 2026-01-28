/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_caps_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 14:13:01 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/27 00:04:55 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	save_intersection_in_table(t_cy_ctx *cy_ctx, float *hit_p,
	double t_side)
{
	float	q[3];
	float	n[3];

	scale(q, cy_ctx->normal, cy_ctx->y);
	add(q, cy_ctx->curr_cy->cy_xyz, q);
	sub(n, hit_p, q);
	normalize(n, n);
	cy_ctx->rec->t = t_side;
	cy_ctx->rec->hit = 1;
	cy_ctx->rec->object_idx = cy_ctx->i;
	cy_ctx->rec->object_type = CYLINDER;
	copy_vectors(cy_ctx->rec->hit_point, hit_p);
	copy_vectors(cy_ctx->rec->normal, n);
	copy_int_vectors(cy_ctx->rec->color, cy_ctx->curr_cy->cy_rgb);
	cy_ctx->rec->obj_scene_idx = cy_ctx->s;
}

/// @brief P = O + tside * D, y = (P - C) * A ∈ [-h , h]
/// @param t_side 
/// @param cy_ctx 
void	compute_cylinder_finite_height(double t_side, t_cy_ctx *cy_ctx)
{
	float	hit_p[3];
	float	scale_td[3];
	float	hitp_minus_center[3];

	scale(scale_td, cy_ctx->d, (float)t_side);
	add(hit_p, cy_ctx->origin, scale_td);
	sub(hitp_minus_center, hit_p, cy_ctx->curr_cy->cy_xyz);
	cy_ctx->y = dot(hitp_minus_center, cy_ctx->normal);
	if (cy_ctx->y >= -cy_ctx->half_height && cy_ctx->y <= cy_ctx->half_height)
	{
		save_intersection_in_table(cy_ctx, hit_p, t_side);
	}
}

/// @brief Solves this equation: v = D - (D * A)A
/// @brief and w = (O - C) - ((O - C) * A)A
/// @brief 1) D = current ray direction 
/// @brief 2) A = Axis-orientation vector
/// @brief 3) C = Center xyz coordinates of the cylinder
/// @brief 4) O = Ray origin (camera xyz coordinates)
/// @param cy_ctx 
/// @return Fill the v and w values inside t_cy_ctx struct
void	calc_v_w(t_cy_ctx *cy_ctx)
{
	float	scale_da_normal[3];
	double	da;
	float	o_minus_c[3];
	float	scale_oca_normal[3];
	double	oca;

	da = dot(cy_ctx->d, cy_ctx->normal);
	scale(scale_da_normal, cy_ctx->normal, (float)da);
	sub(cy_ctx->v, cy_ctx->d, scale_da_normal);
	sub(o_minus_c, cy_ctx->origin, cy_ctx->curr_cy->cy_xyz);
	oca = dot(o_minus_c, cy_ctx->normal);
	scale(scale_oca_normal, cy_ctx->normal, (float)oca);
	sub(cy_ctx->w, o_minus_c, scale_oca_normal);
}

double	solve_cylinder_formula(t_scene	*scene, t_cy_ctx *cy_ctx)
{
	double	t_side;

	init_curr_iter_values(scene, cy_ctx);
	calc_v_w(cy_ctx);
	t_side = solve_t_cylinder(cy_ctx->v, cy_ctx->w, cy_ctx);
	if (t_side > 0.0 && t_side < cy_ctx->rec->t)
	{
		compute_cylinder_finite_height(t_side, cy_ctx);
	}
	cylinder_bottom_cap(cy_ctx, 1);
	cylinder_top_cap(cy_ctx, 1);
	if (cy_ctx->rec->hit)
		return (cy_ctx->rec->t);
	else
		return (-1.0);
}
