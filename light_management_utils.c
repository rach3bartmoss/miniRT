/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_management_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 22:27:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/06 22:30:05 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	normalize_colors(float rgb[3])
{
	rgb[0] /= 255.0f;
	rgb[1] /= 255.0f;
	rgb[2] /= 255.0f;
}

double	ray_length(float vector[3])
{
	double	xyz;

	xyz = (double)vector[0] * vector[0];
	xyz += (double)vector[1] * vector[1];
	xyz += (double)vector[2] * vector[2];
	return (sqrt(xyz));
}

//Color = a * (ca,change / 255) * (co,change / 255)
void	apply_ambient_light(t_scene *scene, t_hit *curr_rec,
	t_render_ctx *render)
{
	float	a;
	float	ca[3];
	float	term;
	int		i;

	a = scene->ambiance->light_ratio;
	ca[0] = (float)scene->ambiance->a_rgb[0];
	ca[1] = (float)scene->ambiance->a_rgb[1];
	ca[2] = (float)scene->ambiance->a_rgb[2];
	render->co[0] = (float)curr_rec->color[0];
	render->co[1] = (float)curr_rec->color[1];
	render->co[2] = (float)curr_rec->color[2];
	normalize_colors(ca);
	normalize_colors(render->co);
	i = 0;
	while (i < 3)
	{
		term = a * ca[i] * render->co[i];
		term = fmaxf(term, 0.0f);
		term = fminf(term, 1.0f);
		render->final_shade[i] = term;
		render->a_shade[i] = (int)(term * 255.0f);
		i++;
	}
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

float	ray_intersection_pl(float *sr_origin, float *sr_dir, t_plane *pl)
{
	float	denom;
	float	plane0_light0[3];
	float	t;

	denom = dot(pl->pl_vector_xyz, sr_dir);
	if (fabsf(denom) < 1e-6f)
		return (-1.0f);
	sub(plane0_light0, pl->pl_xyz, sr_origin);
	t = dot(plane0_light0, pl->pl_vector_xyz) / denom;
	if (t > 0.0f)
		return (t);
	else
		return (-1.0f);
}
