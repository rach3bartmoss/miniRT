/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:01:03 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 23:52:22 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	get_sphere_sr_t(double *p_offset, double *dir, double distance,
		t_scene *scene)
{
	double	curr_sp;
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

int	get_plane_sr_t(double *p_offset, double *dir, double distance,
		t_scene *scene)
{
	double	curr_pl;
	int		p;
	int		count;

	p = 0;
	count = set_and_get_occ(-1, PLANE);
	while (p < count)
	{
		curr_pl = ray_intersection_pl_shadow(p_offset, dir,
				scene->plane[p], distance);
		if (curr_pl > 0.0f && curr_pl < distance)
			return (1);
		++p;
	}
	return (0);
}

int	get_cylinder_sr_t(double *p_offset, double *dir, double distance,
		t_scene *scene)
{
	double	curr_cy;
	int		i;
	int		count;

	i = 0;
	count = set_and_get_occ(-1, CYLINDER);
	while (i < count)
	{
		curr_cy = ray_intersection_cy(p_offset, dir,
				scene->cylinder[i], distance);
		if (curr_cy > 0.0f && curr_cy < distance)
			return (1);
		i++;
	}
	return (0);
}

int	shadow_ray_intersection_dispatcher(double *p_offset, double *dir,
	double distance, t_scene *scene)
{
	int	is_in_shadow;

	is_in_shadow = 0;
	is_in_shadow = get_sphere_sr_t(p_offset, dir, distance, scene);
	if (!is_in_shadow)
		is_in_shadow = get_plane_sr_t(p_offset, dir, distance, scene);
	if (!is_in_shadow)
		is_in_shadow = get_cylinder_sr_t(p_offset, dir, distance, scene);
	if (!is_in_shadow)
		is_in_shadow = get_paraboloid_sr_t(p_offset, dir, distance, scene);
	return (is_in_shadow);
}

/// @brief Generates 3 main params p_offset, light_dir_normalized and distance.
/// @brief This values are used in the shadow ray calculation for each hitpoint
/// @brief (source).
/// @brief 1) p_offset[3];				(SHADOW RAY ORIGIN)
/// @brief 2) light_dir_normalized[3];	(SHADOW RAY DIRECTION NORMALIZED)
/// @brief 3) distance; MAX DISTANCE FROM CURR POINT TO LIGHT SOURCE
/// @param render 
/// @param scene 
/// @param win 
/// @return Return 0 on failure success apply the shadows + ambient light
int	apply_diffuse_specular_and_shadow(t_render_ctx *render,
	t_scene *scene, t_window *win)
{
	double	light_dir_normalized[3];
	double	p_offset[3];
	double	distance;
	int		is_in_shadow;

	(void)win;
	distance = prep_shadow_ray(scene, render, light_dir_normalized, p_offset);
	is_in_shadow = shadow_ray_intersection_dispatcher(p_offset,
			light_dir_normalized, distance, scene);
	if (!is_in_shadow)
		return (combine_lights(render, scene, light_dir_normalized));
	return (0);
}
