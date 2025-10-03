/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_caps.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 23:02:28 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/01 20:54:40 by dopereir         ###   ########.fr       */
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

/// @brief Solves this equations: v = D - (D * A)A
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

int	cylinder_bottom_cap(t_cy_ctx *cy_ctx)
{
	float	bot_center[3];
	float	hit_cap_p[3];
	float	tmp[3];
	float	bot_cap_normal[3];
	float	r = cy_ctx->curr_cy->cy_diameter / 2;
	
	double	denom;
	float	OtoC[3];
	double	tcap;

	//bot_center = base_center= C - hA
	copy_vectors(bot_center, cy_ctx->base_center);
	scale(bot_cap_normal, cy_ctx->normal, -1.0f);

	denom = dot(cy_ctx->d, bot_cap_normal);
	if (fabs(denom) > 1e-6)
	{
		sub(OtoC, bot_center, cy_ctx->origin);
		tcap = dot(OtoC, bot_cap_normal) / denom;
		if (tcap > 0.0 && tcap < cy_ctx->rec->t)
		{
			scale(tmp, cy_ctx->d, (float)tcap);
			add(hit_cap_p, cy_ctx->origin, tmp);
			sub(tmp, hit_cap_p, bot_center);
			if (dot(tmp,tmp) <= (r * r))
			{
				cy_ctx->rec->t = tcap;
				cy_ctx->rec->hit = 1;
				cy_ctx->rec->object_idx = cy_ctx->i;
				copy_vectors(cy_ctx->rec->hit_point, hit_cap_p);
				copy_vectors(cy_ctx->rec->normal, bot_cap_normal);
				copy_int_vectors(cy_ctx->rec->color, cy_ctx->curr_cy->cy_rgb);
			}
		}
	}
	return (1);
}

int	cylinder_top_cap(t_cy_ctx *cy_ctx)
{
	float cap_center[3], hit_cap_p[3], tmp[3];
	float	r = cy_ctx->curr_cy->cy_diameter / 2;
	// cap_center = top_center = C + hA  
	copy_vectors(cap_center, cy_ctx->top_center);
	float cap_n[3]; copy_vectors(cap_n, cy_ctx->normal);

	double denom = dot(cy_ctx->d, cap_n);
	if (fabs(denom) > 1e-6)
	{
		float OtoC[3]; sub(OtoC, cap_center, cy_ctx->origin);
		double tcap = dot(OtoC, cap_n) / denom;

		if (tcap > 0.0 && tcap < cy_ctx->rec->t)
		{
			scale(tmp, cy_ctx->d, (float)tcap);
			add(hit_cap_p, cy_ctx->origin, tmp);
			sub(tmp, hit_cap_p, cap_center);
			if (dot(tmp,tmp) <= (r*r))
			{
				cy_ctx->rec->t = tcap;
				cy_ctx->rec->hit = 1;
				cy_ctx->rec->object_idx = cy_ctx->i;
				copy_vectors(cy_ctx->rec->hit_point, hit_cap_p);
				copy_vectors(cy_ctx->rec->normal, cap_n);
				copy_int_vectors(cy_ctx->rec->color, cy_ctx->curr_cy->cy_rgb);
			}
		}
	}
	return (1);
}
