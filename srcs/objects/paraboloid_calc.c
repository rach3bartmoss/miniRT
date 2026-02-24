/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paraboloid_calc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 14:00:32 by joao-vri          #+#    #+#             */
/*   Updated: 2026/02/23 23:16:42 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/* if discriminant (delta) is negative,
the specific ray doesn't hit the paraboloid */
int	paraboloid_discriminant(t_pa_ctx *curr_ctx)
{
	curr_ctx->a = dot(curr_ctx->d, curr_ctx->d)
		- curr_ctx->dot_dn * curr_ctx->dot_dn;
	curr_ctx->b = 2.0 * (dot(curr_ctx->d, curr_ctx->w) - curr_ctx->dot_dn
			* (curr_ctx->dot_wn + 2.0 * curr_ctx->pa->k));
	curr_ctx->c = dot(curr_ctx->w, curr_ctx->w) - curr_ctx->dot_wn
		* (curr_ctx->dot_wn + 4 * curr_ctx->pa->k);
	curr_ctx->discr = curr_ctx->b * curr_ctx->b
		- (4 * curr_ctx->a * curr_ctx->c);
	if (curr_ctx->discr < 0)
		return (0);
	return (1);
}

double	paraboloid_quadratic(t_pa_ctx *curr_ctx)
{
	double	discr_sqrt;
	double	t1;
	double	t2;

	discr_sqrt = sqrt(curr_ctx->discr);
	t1 = ((curr_ctx->b * -1.0) - discr_sqrt) / (2.0 * curr_ctx->a);
	if (t1 > 0)
	{
		curr_ctx->h = paraboloid_height_test(curr_ctx, t1);
		if (curr_ctx->h >= 0 && curr_ctx->h <= curr_ctx->pa->height)
			return (t1);
	}
	t2 = ((curr_ctx->b * -1.0) + discr_sqrt) / (2.0 * curr_ctx->a);
	if (t2 > 0)
	{
		curr_ctx->h = paraboloid_height_test(curr_ctx, t2);
		if (curr_ctx->h >= 0 && curr_ctx->h <= curr_ctx->pa->height)
			return (t2);
	}
	return (-1.0);
}

double	paraboloid_height_test(t_pa_ctx *curr_ctx, double tx)
{
	double	p[3];
	double	p_v_sub[3];
	double	h;

	p[0] = curr_ctx->o[0] + (tx * curr_ctx->d[0]);
	p[1] = curr_ctx->o[1] + (tx * curr_ctx->d[1]);
	p[2] = curr_ctx->o[2] + (tx * curr_ctx->d[2]);
	sub(p_v_sub, p, curr_ctx->pa->center);
	h = dot(p_v_sub, curr_ctx->n);
	return (h);
}

// N=normalize(Vrel​​−(h+k).Axis)
void	paraboloid_normal(t_pa_ctx *curr_ctx, double t)
{
	double	v_rel[3];
	double	new_axis[3];
	double	actual_h;

	curr_ctx->p[0] = curr_ctx->o[0] + t * curr_ctx->d[0];
	curr_ctx->p[1] = curr_ctx->o[1] + t * curr_ctx->d[1];
	curr_ctx->p[2] = curr_ctx->o[2] + t * curr_ctx->d[2];
	sub(v_rel, curr_ctx->p, curr_ctx->pa->center);
	actual_h = dot(v_rel, curr_ctx->pa->axis);
	new_axis[0] = curr_ctx->pa->axis[0] * (actual_h + curr_ctx->pa->k * 2.0);
	new_axis[1] = curr_ctx->pa->axis[1] * (actual_h + curr_ctx->pa->k * 2.0);
	new_axis[2] = curr_ctx->pa->axis[2] * (actual_h + curr_ctx->pa->k * 2.0);
	sub(curr_ctx->n_surface, v_rel, new_axis);
	normalize(curr_ctx->n_surface, curr_ctx->n_surface);
	if (dot(curr_ctx->d, curr_ctx->n_surface) > 0)
	{
		curr_ctx->n_surface[0] *= -1.0;
		curr_ctx->n_surface[1] *= -1.0;
		curr_ctx->n_surface[2] *= -1.0;
	}
}
