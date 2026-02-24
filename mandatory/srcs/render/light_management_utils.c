/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_management_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 22:27:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/09 20:05:21 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

int	combine_lights(t_render_ctx *render, t_scene *scene, float *dir)
{
	float	obj_color[3];
	float	light_color[3];
	float	diffuse_part[3];
	float	diffuse_factor;
	int		j;

	normalize_target_colors(obj_color, render->rec->color);
	normalize_target_colors(light_color, scene->light->light_rgb);
	diffuse_factor = fmaxf(0.0f, dot(render->rec->normal, dir));
	mult(diffuse_part, obj_color, light_color);
	scale(diffuse_part, diffuse_part,
		scene->light->bright_ratio * diffuse_factor);
	add(render->final_shade, render->final_shade, diffuse_part);
	j = 0;
	while (j < 3)
	{
		render->final_shade[j] = fmaxf(render->final_shade[j], 0.0f);
		render->final_shade[j] = fminf(render->final_shade[j], 1.0f);
		render->out_shade[j] = (int)(render->final_shade[j] * 255.0f);
		j++;
	}
	return (1);
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
