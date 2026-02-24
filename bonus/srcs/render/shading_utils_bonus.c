/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:44:13 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/06 21:25:08 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

int	validate_cylinder_shade_to_pixel(t_app *app, t_hit *hit)
{
	if (hit->obj_scene_idx < app->scene->cylinder_capacity
		&& app->scene->cylinder[hit->obj_scene_idx] != NULL
		&& app->scene->cylinder[hit->obj_scene_idx]->checkerboard)
		return (apply_checkerboard(hit));
	else
		return (rgb3_to_hex(hit->color));
}

int	apply_shade_to_pixel(t_app *app, t_hit *hit)
{
	if (hit->object_type == PLANE)
	{
		if (hit->obj_scene_idx < app->scene->plane_capacity
			&& app->scene->plane[hit->obj_scene_idx] != NULL
			&& app->scene->plane[hit->obj_scene_idx]->checkerboard)
			return (apply_checkerboard(hit));
		else
			return (rgb3_to_hex(hit->color));
	}
	if (hit->object_type == SPHERE)
	{
		if (hit->obj_scene_idx <= app->scene->sphere_capacity
			&& app->scene->sphere[hit->obj_scene_idx] != NULL
			&& app->scene->sphere[hit->obj_scene_idx]->checkerboard == 1)
			return (apply_checkerboard(hit));
		else
			return (rgb3_to_hex(hit->color));
	}
	if (hit->object_type == CYLINDER)
		return (validate_cylinder_shade_to_pixel(app, hit));
	return (0x000000);
}
