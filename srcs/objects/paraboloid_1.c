/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paraboloid_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 14:00:32 by joao-vri          #+#    #+#             */
/*   Updated: 2026/02/23 21:24:59 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_paraboloid_ctx(t_paraboloid *curr_pa, t_pa_ctx *curr_ctx, t_scene *scene, t_ray_table *ray_table, int i)
{
	if (!curr_pa || !curr_ctx || !scene)
		return;
	curr_ctx->pa = curr_pa;
	copy_vectors(curr_ctx->n, curr_pa->axis);
	copy_vectors(curr_ctx->o, scene->camera->coordinates_xyz);
	curr_ctx->d[0] = ray_table->vectors_x[i];
	curr_ctx->d[1] = ray_table->vectors_y[i];
	curr_ctx->d[2] = ray_table->vectors_z[i];
	sub(curr_ctx->w, curr_ctx->o, curr_pa->center);
	curr_ctx->dot_dn = dot(curr_ctx->d, curr_ctx->n);
	curr_ctx->dot_wn = dot(curr_ctx->w, curr_ctx->n);
}

void	init_shadow_paraboloid(t_paraboloid *curr_pa, t_pa_ctx *curr_ctx, double *new_origin, double *dir)
{
	if (!curr_pa || !curr_ctx)
		return;
	curr_ctx->pa = curr_pa;
	copy_vectors(curr_ctx->n, curr_pa->axis);
	copy_vectors(curr_ctx->o, new_origin);
	copy_vectors(curr_ctx->d, dir);
	sub(curr_ctx->w, curr_ctx->o, curr_pa->center);
	curr_ctx->dot_dn = dot(curr_ctx->d, curr_ctx->n);
	curr_ctx->dot_wn = dot(curr_ctx->w, curr_ctx->n);
}
// if discriminant (delta) is negative, the specific ray doesn't hit the paraboloid
int	paraboloid_discriminant(t_pa_ctx *curr_ctx)
{
	curr_ctx->a = dot(curr_ctx->d, curr_ctx->d) - curr_ctx->dot_dn * curr_ctx->dot_dn;
	curr_ctx->b = 2.0 * (dot(curr_ctx->d, curr_ctx->w) - curr_ctx->dot_dn * (curr_ctx->dot_wn + 2.0 * curr_ctx->pa->k));
	curr_ctx->c = dot(curr_ctx->w, curr_ctx->w) - curr_ctx->dot_wn * (curr_ctx->dot_wn + 4 * curr_ctx->pa->k);
	curr_ctx->discr = curr_ctx->b * curr_ctx->b - (4 * curr_ctx->a * curr_ctx->c);
	if (curr_ctx->discr < 0)
		return (0);
	return (1);
}

double	paraboloid_quadratic(t_pa_ctx *curr_ctx)
{
	double	discr_sqrt;
	double	t1;
	double	t2;
	double	height_test;

	discr_sqrt = sqrt(curr_ctx->discr);
	t1 = ((curr_ctx->b * -1.0) - discr_sqrt) / (2.0 * curr_ctx->a);
	if (t1 > 0)
	{
		height_test = paraboloid_height_test(curr_ctx, t1);
		if (height_test >= 0 && height_test <= curr_ctx->pa->height)
		{
			curr_ctx->h = height_test;
			return (t1);
		}
	}
	t2 = ((curr_ctx->b * -1.0) + discr_sqrt) / (2.0 * curr_ctx->a);
	if (t2 > 0)
	{
		height_test = paraboloid_height_test(curr_ctx, t2);
		if (height_test >= 0 && height_test <= curr_ctx->pa->height)
		{
			curr_ctx->h = height_test;
			return (t2);
		}
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

void	ray_paraboloid_intersection(t_ray_table *ray_table, t_scene *scene, int shadow)
{
	int		i;
	int		j;
	int		pa_count;
	double	t;
	t_pa_ctx	ctx;

	shadow++;
	pa_count = set_and_get_occ(-1, PARABOLOID);
	i = 0;
	while (i < ray_table->total_rays)
	{
		j = 0;
		while (j < pa_count)
		{
			init_paraboloid_ctx(scene->paraboloid[j], &ctx, scene, ray_table, i);
			if (paraboloid_discriminant(&ctx))
			{
				t = paraboloid_quadratic(&ctx);
				if (t > 0.000001 && t < ray_table->hit_record[i].t)
				{
					ray_table->hit_record[i].t = t;
					ray_table->hit_record[i].hit = 1;
					ray_table->hit_record[i].object_type = PARABOLOID;
					copy_int_vectors(ray_table->hit_record[i].color, scene->paraboloid[j]->rgb);
					paraboloid_normal(&ctx, t);
					copy_vectors(ray_table->hit_record[i].hit_point, ctx.p);
					copy_vectors(ray_table->hit_record[i].normal, ctx.n_surface);
					ray_table->hit_record[i].obj_scene_idx = j;
				}
			}
			j++;
		}
		i++;
	}
}
