/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_checkerboard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 22:13:12 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 21:25:11 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

int	apply_checkerboard(t_hit *hit)
{
	int		rgb[3];
	int		ix;
	int		iz;

	ix = floorf(hit->hit_point[0] / SQUARE_PATTERN_SCALE);
	iz = floorf(hit->hit_point[2] / SQUARE_PATTERN_SCALE);
	if ((ix + iz) % 2 == 0)
	{
		rgb[0] = 255;
		rgb[1] = 255;
		rgb[2] = 255;
	}
	else
	{
		rgb[0] = 0;
		rgb[1] = 0;
		rgb[2] = 0;
	}
	return (rgb3_to_hex(rgb));
}

void	apply_checkboard_for_sphere(t_hit *hit, t_sphere *sp, int target[3])
{
	t_ckboard_sp_ctx	ctx;

	ctx.radius = sp->sp_diameter / 2.0f;
	ctx.p[0] = (hit->hit_point[0] - sp->sp_center_xyz[0]) / ctx.radius;
	ctx.p[1] = (hit->hit_point[1] - sp->sp_center_xyz[1]) / ctx.radius;
	ctx.p[2] = (hit->hit_point[2] - sp->sp_center_xyz[2]) / ctx.radius;
	ctx.theta = atan2f(ctx.p[2], ctx.p[0]);
	ctx.phi = acosf(ctx.p[1]);
	ctx.u = (ctx.theta + M_PI) / (2.0f * M_PI);
	ctx.v = ctx.phi / M_PI;
	ctx.u_i = floorf(ctx.u * SQUARE_PATTERN_SCALE);
	ctx.v_i = floorf(ctx.v * SQUARE_PATTERN_SCALE);
	if ((ctx.u_i + ctx.v_i) % 2 == 0)
		set_vec_int_values(target, 255, 255, 255);
	else
		set_vec_int_values(target, 0, 0, 0);
}

void	apply_checkerboard_for_plane(t_hit *hit, t_plane *pl, int target[3])
{
	int	a;
	int	b;

	if (fabs(pl->pl_vector_xyz[1]) > 0.9f)
	{
		a = floorf(hit->hit_point[0] / SQUARE_PATTERN_SCALE);
		b = floorf(hit->hit_point[2] / SQUARE_PATTERN_SCALE);
	}
	else if (fabs(pl->pl_vector_xyz[0]) > 0.9f)
	{
		a = floorf(hit->hit_point[1] / SQUARE_PATTERN_SCALE);
		b = floorf(hit->hit_point[2] / SQUARE_PATTERN_SCALE);
	}
	else
	{
		a = floorf(hit->hit_point[0] / SQUARE_PATTERN_SCALE);
		b = floorf(hit->hit_point[1] / SQUARE_PATTERN_SCALE);
	}
	if ((a + b) % 2 == 0)
		set_vec_int_values(target, 255, 255, 255);
	else
		set_vec_int_values(target, 0, 0, 0);
}

/// @brief Apply checkerboard pattern into 3D cylinders
/// @brief as used in The Ray Tracing Challenger book.
/// @param hit 
/// @param cy 
/// @param target 
void	apply_checkerboard_cy(t_hit *hit, t_cylinder *cy, int target[3])
{
	t_ckboard_cy_ctx	ctx;

	fill_inv_matrix(ctx.mx, cy);
	apply_matrix(ctx.p, ctx.mx, hit->hit_point);
	ctx.tetha = atan2f(ctx.p[0], ctx.p[2]);
	ctx.raw_u = ctx.tetha / (2.0f * M_PI);
	ctx.u = 1.0f - (ctx.raw_u + 0.5f);
	ctx.v = ctx.p[1];
	ctx.v = fmodf(ctx.v, 1.0f);
	if (ctx.v < 0.0f)
		ctx.v += 1.0f;
	ctx.iu = (int)floorf(ctx.u * 20.0f);
	ctx.iv = (int)floorf(ctx.v * 2.0f);
	if ((ctx.iu + ctx.iv) % 2 == 0)
		set_vec_int_values(target, 255, 255, 255);
	else
		set_vec_int_values(target, 0, 0, 0);
}
