/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ginfranc <ginfranc@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:35:32 by dopereir          #+#    #+#             */
/*   Updated: 2025/11/22 13:43:58 by ginfranc         ###   ########.fr       */
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
		free(scene->sphere);
		printf("miniRT: Plane initialization failed.\n");
		return (0);
	}
	scene->plane_capacity = OBJ_INITIAL_CAPACITY;
	scene->cylinder = ft_calloc(OBJ_INITIAL_CAPACITY, sizeof(t_cylinder *));
	if (!scene->cylinder)
	{
		free(scene->sphere);
		free(scene->plane);
		printf("miniRT: Cylinder initialization failed.\n");
		return (0);
	}
	scene->cylinder_capacity = OBJ_INITIAL_CAPACITY;
	return (1);
}
