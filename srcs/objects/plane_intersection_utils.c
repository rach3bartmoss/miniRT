/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_intersection_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:47:34 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 21:24:59 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// DEPRECATED FUNCTION
/*int	plane_occludes(double origin[3], double dir[3], double max_t, t_pl_ctx *P)
{
	double	diff[3];
	double	denom;
	double	t;

	denom = dot(dir, P->normal);
	if (fabs(denom) < 1e-6f)
		return (-1);
	sub(diff, P->p0, origin);
	t = dot(diff, P->normal) / denom;
	return (t > SHADOW_EPS && t < max_t);
}*/

static void	create_iter_pl_ctx(t_scene *scene, t_pl_ctx *pl_ctx)
{
	copy_vectors(pl_ctx->normal, pl_ctx->curr_pl->pl_vector_xyz);
	copy_vectors(pl_ctx->origin, scene->camera->coordinates_xyz);
	copy_vectors(pl_ctx->p0, pl_ctx->curr_pl->pl_xyz);
	pl_ctx->denom = dot(pl_ctx->d, pl_ctx->normal);
}

//ray:		R(t) = O + tD
//plane:	(X - P0) * n = 0
/*	together:
			(O + td - P0) * n = 0
	t	=	(Po - O) * n / D * n

	if (D * N) = 0 ray is parallel to the plane
*/
//store into scale_tD = (t*D)
//hit_p = (O + t * D)
double	solve_pl_formula(t_scene *scene, t_pl_ctx *pl_ctx, int flag)
{
	double	t;

	if (flag == 1)
		create_iter_pl_ctx(scene, pl_ctx);
	if (fabs(pl_ctx->denom) < 1e-6)
		return (0.0f);
	sub(pl_ctx->diff, pl_ctx->p0, pl_ctx->origin);
	t = dot(pl_ctx->diff, pl_ctx->normal) / pl_ctx->denom;
	if (t < 0.0f)
		return (-1.0f);
	if (flag == 1 && t < pl_ctx->rec->t)
	{
		pl_ctx->rec->t = t;
		pl_ctx->rec->hit = 1;
		pl_ctx->rec->object_idx = pl_ctx->i;
		pl_ctx->rec->object_type = PLANE;
		pl_ctx->rec->obj_scene_idx = pl_ctx->s;
		scale(pl_ctx->scale_td, pl_ctx->d, t);
		add(pl_ctx->hit_p, pl_ctx->origin, pl_ctx->scale_td);
		copy_vectors(pl_ctx->rec->hit_point, pl_ctx->hit_p);
		copy_vectors(pl_ctx->rec->normal, pl_ctx->normal);
		copy_vectors(pl_ctx->rec->shading_normal, pl_ctx->normal);
		copy_int_vectors(pl_ctx->rec->color, pl_ctx->curr_pl->pl_rgb);
	}
	return (t);
}
