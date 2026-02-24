/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:35:32 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/29 12:20:31 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

int	init_paraboloid(t_scene *scene)
{
	scene->paraboloid = ft_calloc(OBJ_INITIAL_CAPACITY, sizeof(t_paraboloid *));
	if (!scene->paraboloid)
	{
		printf("miniRT: Paraboloid initialization failed.\n");
		return (0);
	}
	scene->paraboloid_capacity = OBJ_INITIAL_CAPACITY;
	return (1);
}

int	init_objects(t_scene *scene)
{
	scene->sphere = ft_calloc(OBJ_INITIAL_CAPACITY, sizeof(t_sphere *));
	if (!scene->sphere)
	{
		printf("miniRT: Sphere initialization failed.\n");
		return (0);
	}
	scene->sphere_capacity = OBJ_INITIAL_CAPACITY;
	scene->plane = ft_calloc(OBJ_INITIAL_CAPACITY, sizeof(t_plane *));
	if (!scene->plane)
	{
		printf("miniRT: Plane initialization failed.\n");
		return (0);
	}
	scene->plane_capacity = OBJ_INITIAL_CAPACITY;
	scene->cylinder = ft_calloc(OBJ_INITIAL_CAPACITY, sizeof(t_cylinder *));
	if (!scene->cylinder)
	{
		printf("miniRT: Cylinder initialization failed.\n");
		return (0);
	}
	scene->cylinder_capacity = OBJ_INITIAL_CAPACITY;
	if (init_paraboloid(scene) == 0)
		return (0);
	return (1);
}
