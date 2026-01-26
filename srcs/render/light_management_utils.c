/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_management_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 22:27:10 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/23 23:49:40 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// (N (surface) * 1e-4)
//(Light pos - P_origin)
//gets (N * ε)
float	prep_shadow_ray(t_scene *scene, t_render_ctx *render, float *dir,
		float *p_offset)
{
	float	ntimes_epsilon[3];
	float	l_pos_p[3];

	sub(dir, scene->light->light_xyz, render->rec->hit_point);
	normalize(dir, dir);
	scale(ntimes_epsilon, render->rec->normal, SHADOW_EPS);
	add(p_offset, render->rec->hit_point, ntimes_epsilon);
	sub(l_pos_p, scene->light->light_xyz, render->rec->hit_point);
	return (ray_length(l_pos_p));
}

float	ray_intersection_sp(float *sr_origin, float *sr_dir,
	t_sphere *sphere)
{
	t_sp_ctx	sp_ctx;
	float		t;

	sp_ctx.curr_sp = sphere;
	t = solve_discriminant(sr_origin, sr_dir, &sp_ctx, 0);
	return (t);
}

double	ray_intersection_pl_shadow(float *sr_origin, float *sr_dir, t_plane *pl,
	float distance)
{
	double	denom;
	float	plane0_light0[3];
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
