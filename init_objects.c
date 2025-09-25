/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:35:32 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/25 14:38:57 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
	return (1);
}
