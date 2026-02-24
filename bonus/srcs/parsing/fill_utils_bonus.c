/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 22:29:17 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/29 22:42:40 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

size_t	get_elem_size(t_type_elem type)
{
	if (type == AMBIENT)
		return (sizeof(t_ambient));
	if (type == CAMERA)
		return (sizeof(t_camera));
	if (type == LIGHT)
		return (sizeof(t_light));
	if (type == SPHERE)
		return (sizeof(t_sphere));
	if (type == PLANE)
		return (sizeof(t_plane));
	if (type == CYLINDER)
		return (sizeof(t_cylinder));
	return (0);
}

char	*get_type_name(t_type_elem type)
{
	if (type == AMBIENT)
		return ("AMBIENT");
	if (type == CAMERA)
		return ("CAMERA");
	if (type == LIGHT)
		return ("LIGHT");
	if (type == SPHERE)
		return ("SPHERE");
	if (type == PLANE)
		return ("PLANE");
	if (type == CYLINDER)
		return ("CYLINDER");
	if (type == PARABOLOID)
		return ("PARABOLOID");
	return ("UNKNOWN");
}
