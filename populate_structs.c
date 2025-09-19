/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_structs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 22:27:05 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/19 23:47:41 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief 
/// @param id 
/// @param scene 
/// @param line 
/// @return Return 0 on failure, 1 on success.
int	check_element_id(char *id, t_scene *scene, char *line)
{
	int	rc;

	rc = 0;
	if (ft_strcmp(id, "A") == 0)
		rc = fill_ambiance(scene, line);
	if (ft_strcmp(id, "C") == 0)
		rc = fill_camera(scene, line);
	if (ft_strcmp(id, "L") == 0)
		rc = fill_light(scene, line);
	if (ft_strcmp(id, "sp") == 0)
		rc = fill_sphere(scene, line);
	/*if (ft_strcmp(id, "pl") == 0)
		rc = fill_plane(scene, line);
	if (ft_strcmp(id, "cy") == 0)
		rc = fill_cylinder(scene, line);*/
	return (rc);
}

int	tokenize_line(char *line, t_scene *scene)
{
	char	*id;

	(void) scene;
	id = ft_strtok(line, " \t\v\f\r");
	check_element_id(id, scene, line);
	
	return (1);
}