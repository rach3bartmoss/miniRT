/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paraboloid_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 22:57:45 by joao-vri          #+#    #+#             */
/*   Updated: 2026/02/23 23:17:54 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_paraboloid_ctx(t_paraboloid *curr_pa, t_pa_ctx *curr_ctx,
	t_scene *scene, t_ray_table *ray_table)
{
	if (!curr_pa || !curr_ctx || !scene)
		return ;
	curr_ctx->pa = curr_pa;
	copy_vectors(curr_ctx->n, curr_pa->axis);
	copy_vectors(curr_ctx->o, scene->camera->coordinates_xyz);
	curr_ctx->d[0] = ray_table->vectors_x[curr_ctx->i];
	curr_ctx->d[1] = ray_table->vectors_y[curr_ctx->i];
	curr_ctx->d[2] = ray_table->vectors_z[curr_ctx->i];
	sub(curr_ctx->w, curr_ctx->o, curr_pa->center);
	curr_ctx->dot_dn = dot(curr_ctx->d, curr_ctx->n);
	curr_ctx->dot_wn = dot(curr_ctx->w, curr_ctx->n);
}

void	init_shadow_paraboloid(t_paraboloid *curr_pa, t_pa_ctx *curr_ctx,
	double *new_origin, double *dir)
{
	if (!curr_pa || !curr_ctx)
		return ;
	curr_ctx->pa = curr_pa;
	copy_vectors(curr_ctx->n, curr_pa->axis);
	copy_vectors(curr_ctx->o, new_origin);
	copy_vectors(curr_ctx->d, dir);
	sub(curr_ctx->w, curr_ctx->o, curr_pa->center);
	curr_ctx->dot_dn = dot(curr_ctx->d, curr_ctx->n);
	curr_ctx->dot_wn = dot(curr_ctx->w, curr_ctx->n);
}
