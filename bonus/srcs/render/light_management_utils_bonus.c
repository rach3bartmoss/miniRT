/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_management_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 22:27:10 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 23:52:13 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

// (N (surface) * 1e-4)
//(Light pos - P_origin)
//gets (N * ε)
double	prep_shadow_ray(t_scene *scene, t_render_ctx *render, double *dir,
		double *p_offset)
{
	double	ntimes_epsilon[3];
	double	l_pos_p[3];

	sub(dir, scene->light->light_xyz, render->rec->hit_point);
	normalize(dir, dir);
	scale(ntimes_epsilon, render->rec->normal, SHADOW_EPS);
	add(p_offset, render->rec->hit_point, ntimes_epsilon);
	sub(l_pos_p, scene->light->light_xyz, render->rec->hit_point);
	return (ray_length(l_pos_p));
}

double	ray_intersection_sp(double *sr_origin, double *sr_dir,
	t_sphere *sphere)
{
	t_sp_ctx	sp_ctx;
	double		t;

	sp_ctx.curr_sp = sphere;
	t = solve_discriminant(sr_origin, sr_dir, &sp_ctx, 0);
	return (t);
}

double	ray_intersection_pl_shadow(double *sr_origin, double *sr_dir,
	t_plane *pl, double distance)
{
	double	denom;
	double	plane0_light0[3];
	double	t;

	denom = dot(pl->pl_vector_xyz, sr_dir);
	if (fabs(denom) <= 1e-4)
		return (-1.0);
	sub(plane0_light0, pl->pl_xyz, sr_origin);
	t = dot(plane0_light0, pl->pl_vector_xyz) / denom;
	if (t > 1e-4 && t < (double)(distance - 1e-4))
		return (t);
	else
		return (-1.0);
}

double	ray_intersection_pa_shadow(double *new_origin,
	double *new_dir, t_paraboloid *pa)
{
	double		t;
	t_pa_ctx	shadow_ctx;

	init_shadow_paraboloid(pa, &shadow_ctx, new_origin, new_dir);
	if (paraboloid_discriminant(&shadow_ctx))
	{
		t = paraboloid_quadratic(&shadow_ctx);
		if (t > 0.0001)
			return (t);
	}
	return (-1.0);
}

int	get_paraboloid_sr_t(double *new_origin, double *new_dir, double distance,
		t_scene *scene)
{
	double	curr_pa;
	int		i;
	int		count;

	i = 0;
	count = set_and_get_occ(-1, PARABOLOID);
	while (i < count)
	{
		curr_pa = ray_intersection_pa_shadow(new_origin,
				new_dir, scene->paraboloid[i]);
		if (curr_pa > 0.0f && curr_pa < distance)
			return (1);
		i++;
	}
	return (0);
}
