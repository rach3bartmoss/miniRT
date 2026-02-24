/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 21:41:58 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/29 23:09:26 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

/// @brief Free a array of string created by ft_split()
/// @param parts The target array.
void	free_split(char **parts)
{
	size_t	i;

	i = 0;
	if (!parts)
		return ;
	while (parts[i])
	{
		free(parts[i]);
		i++;
	}
	free(parts);
}

void	sanitize_gnl(int fd)
{
	char	*tmp;

	tmp = get_next_line(fd);
	while (tmp != NULL)
	{
		free(tmp);
		tmp = get_next_line(fd);
	}
	free (tmp);
}

void	clean_lights_and_camera(t_scene *scene)
{
	if (scene->ambiance)
	{
		free (scene->ambiance);
		scene->ambiance = NULL;
	}
	if (scene->camera)
	{
		free (scene->camera);
		scene->camera = NULL;
	}
	if (scene->light)
	{
		free (scene->light);
		scene->light = NULL;
	}
}

void	clean_sp_pl(t_scene *scene)
{
	int	count;
	int	i;

	count = set_and_get_occ(-1, SPHERE);
	i = 0;
	while (i < count)
	{
		free(scene->sphere[i]->id);
		free(scene->sphere[i]);
		i++;
	}
	free (scene->sphere);
	i = 0;
	count = set_and_get_occ(-1, PLANE);
	while (i < count)
	{
		free(scene->plane[i]->id);
		free(scene->plane[i]);
		i++;
	}
	free(scene->plane);
}

void	cleanup_all(t_scene *scene)
{
	clean_lights_and_camera(scene);
	clean_sp_pl(scene);
	clean_paraboloid_and_cylinder(scene);
	ft_strtok_free();
}
