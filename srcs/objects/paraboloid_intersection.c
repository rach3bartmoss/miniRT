/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paraboloid_intersection.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 22:56:35 by joao-vri          #+#    #+#             */
/*   Updated: 2026/02/23 23:36:15 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	paraboloid_intersection_helper(t_ray_table *ray_table,
		t_scene *scene, t_pa_ctx ctx, int j)
{
	ray_table->hit_record[ctx.i].t = ctx.t;
	ray_table->hit_record[ctx.i].hit = 1;
	ray_table->hit_record[ctx.i].object_type = PARABOLOID;
	copy_int_vectors(ray_table->hit_record[ctx.i].color,
		scene->paraboloid[j]->rgb);
	paraboloid_normal(&ctx, ctx.t);
	copy_vectors(ray_table->hit_record[ctx.i].hit_point, ctx.p);
	copy_vectors(ray_table->hit_record[ctx.i].normal, ctx.n_surface);
	ray_table->hit_record[ctx.i].obj_scene_idx = j;
}

void	ray_paraboloid_intersection(t_ray_table *ray_table, t_scene *scene)
{
	int			j;
	int			pa_count;
	t_pa_ctx	ctx;

	pa_count = set_and_get_occ(-1, PARABOLOID);
	ctx.i = 0;
	while (ctx.i < ray_table->total_rays)
	{
		j = 0;
		while (j < pa_count)
		{
			init_paraboloid_ctx(scene->paraboloid[j],
				&ctx, scene, ray_table);
			if (paraboloid_discriminant(&ctx))
			{
				ctx.t = paraboloid_quadratic(&ctx);
				if (ctx.t > 0.000001 && ctx.t < ray_table->hit_record[ctx.i].t)
					paraboloid_intersection_helper(ray_table, scene, ctx, j);
			}
			j++;
		}
		ctx.i++;
	}
}
