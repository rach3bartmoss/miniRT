/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:35:32 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/19 22:44:25 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	init_objects(t_scene *scene)
{
	scene->sphere = malloc(sizeof(t_sphere *) * OBJ_INITIAL_CAPACITY);
	if (!scene->sphere)
	{
		printf("miniRT: Sphere initialization failed.\n");
		return (0);
	}
	scene->sphere_capacity = OBJ_INITIAL_CAPACITY;
	return (1);
}
