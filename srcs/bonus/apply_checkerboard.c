/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_checkerboard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 22:13:12 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/27 23:29:01 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

	if (fabsf(pl->pl_vector_xyz[1]) > 0.9f)
	{
		a = floorf(hit->hit_point[0] / SQUARE_PATTERN_SCALE);
		b = floorf(hit->hit_point[2] / SQUARE_PATTERN_SCALE);
	}
	else if (fabsf(pl->pl_vector_xyz[0]) > 0.9f)
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

void	apply_checkerboard_for_cy(t_hit *hit, t_cylinder *cy, int target[3])
{
	float	d[3];
	float	radial[3];
	float	axis_u[3], axis_v[3];
	float	u;
	float	v;
	int	iu;
	int	iv;

	sub(d, hit->hit_point, cy->cy_xyz);

	v = dot(d, cy->cy_vector_xyz);

	radial[0] = d[0] - v * cy->cy_vector_xyz[0];
	radial[1] = d[1] - v * cy->cy_vector_xyz[1];
	radial[2] = d[2] - v * cy->cy_vector_xyz[2];

	if (fabsf(cy->cy_vector_xyz[1]) < 0.9f)
		axis_u[0]=0, axis_u[1]=1, axis_u[2]=0;
	else
		axis_u[0]=1, axis_u[1]=0, axis_u[2]=0;

	cross(cy->cy_vector_xyz, axis_u, axis_u);
	normalize(axis_u, axis_u);

	cross(cy->cy_vector_xyz, axis_u, axis_v);
	normalize(axis_v, axis_v);

	// u = angular coordinate
	u = atan2f(dot(radial, axis_v), dot(radial, axis_u));

	// Checker indices
	float u01 = (u + M_PI) / (2.0f * M_PI);
	float u_len = u01 * (2.0f * M_PI * (cy->cy_diameter / 2.0f));

	iu = floorf(u_len / SQUARE_PATTERN_SCALE);
	iv = floorf(v     / SQUARE_PATTERN_SCALE);


	if ((iu + iv) % 2 == 0)
	{
		target[0] = 255;
		target[1] = 255;
		target[2] = 255;
	}
	else {
		target[0] = 0;
		target[1] = 0;
		target[2] = 0;
	}
}
