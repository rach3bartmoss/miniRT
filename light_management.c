/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:01:03 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/08 23:57:52 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	prep_shadow_ray(t_scene *scene, t_render_ctx *render, float *dir,
		float *p_offset)
{
	float	ntimes_epsilon[3];// (N (surface) * 1e-4)
	float	l_pos_p[3];//(Light pos - P_origin)

	sub(dir, scene->light->light_xyz, render->rec->hit_point);
	normalize(dir, dir);
	scale(ntimes_epsilon, render->rec->normal, SHADOW_EPS);//gets (N * ε)
	add(p_offset, render->rec->hit_point, ntimes_epsilon);
	sub(l_pos_p, scene->light->light_xyz, render->rec->hit_point);
	return (ray_length(l_pos_p));//distance from hit_point to light point
}

int	get_sphere_sr_t(float *p_offset, float *dir, float distance,
		t_scene *scene)
{
	float	curr_sp;
	int		i;
	int		count;

	i = 0;
	count = set_and_get_occ(-1, SPHERE);
	while (i < count)
	{
		curr_sp = ray_intersection_sp(p_offset, dir, scene->sphere[i]);
		if (curr_sp > 0.0f && curr_sp < distance)
			return (1);
		i++;
	}
	return (0);
}

int	get_plane_sr_t(float *p_offset, float *dir, float distance,
		t_scene *scene)
{
	double	curr_pl;
	//int		i;
	int		count;

	//i = 0;
	int	p = 0;
	count = set_and_get_occ(-1, PLANE);
	while (p < count)
	{
		curr_pl = ray_intersection_pl_shadow(p_offset, dir, scene->plane[p], distance);
		if (curr_pl > 0.0f && curr_pl < distance)
			return (1);
		++p;
	}
	return (0);
}

int	get_cylinder_sr_t(float *p_offset, float *dir, float distance,
		t_scene *scene)
{
	double	curr_cy;
	int		i;
	int		count;

	i = 0;
	count = set_and_get_occ(-1, CYLINDER);
	while (i < count)
	{
		curr_cy = ray_intersection_cy(p_offset, dir, scene->cylinder[i], distance);
		if (curr_cy > 0.0f && curr_cy < distance)
			return (1);
		i++;
	}
	return (0);
}

int	shadow_ray_intersection_dispatcher(float *p_offset, float *dir, float distance,
		t_scene *scene)
{
	int	is_in_shadow;

	is_in_shadow = 0;
	is_in_shadow = get_sphere_sr_t(p_offset, dir, distance, scene);
	if (!is_in_shadow)
		is_in_shadow = get_plane_sr_t(p_offset, dir, distance, scene);
	if (!is_in_shadow)
		is_in_shadow = get_cylinder_sr_t(p_offset, dir, distance, scene);
	return (is_in_shadow);
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
	scale(diffuse_part, diffuse_part, scene->light->bright_ratio * diffuse_factor);
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

int	apply_diffuse_and_shadow(t_render_ctx *render, t_scene *scene, t_window *win)
{
	(void)win;
	float	light_dir_normalized[3]; //ACTUAL SHADOW RAY DIRECTION NORMALIZED
	float	p_offset[3]; //ACTUAL SHADOW RAY ORIGIN
	float	distance; //MAX DISTANCE FROM CURR POINT TO LIGHT SOURCE
	int		is_in_shadow;

	//1 PART - get ray origin and direction
	distance = prep_shadow_ray(scene, render, light_dir_normalized, p_offset);

	//2 part - test intersections
	is_in_shadow = 0;

	is_in_shadow = shadow_ray_intersection_dispatcher(p_offset,
		light_dir_normalized, distance, scene);

	//3 part - calculate final color
	if (!is_in_shadow)
		return (combine_lights(render, scene, light_dir_normalized));
	return (0);
}
