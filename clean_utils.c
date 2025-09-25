/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 21:41:58 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/20 19:30:17 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief Free a array of string created by ft_split()
/// @param parts The target array.
void	free_split(char **parts)
{
	size_t	i;

	i = 0;
	if (!parts)
		return;
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

	while ((tmp = get_next_line(fd)) != NULL)
		free (tmp);
}

void	cleanup_all(t_scene *scene)
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
		scene->camera = NULL;
	}
	int	count = set_and_get_occ(-1, SPHERE);
	for (int i = 0; i < count; i++)
	{
		free(scene->sphere[i]->id);
		free(scene->sphere[i]);
	}
	free (scene->sphere);
	
	count = set_and_get_occ(-1, PLANE);
	for (int i = 0; i < count; i++)
	{
		free(scene->plane[i]->id);
		free(scene->plane[i]);
	}
	free(scene->plane);
	
	count = set_and_get_occ(-1, CYLINDER);
	for (int i = 0; i < count; i++)
	{
		free(scene->cylinder[i]->id);
		free(scene->cylinder[i]);
	}
	free(scene->cylinder);
	ft_strtok_free();
	//gnl_free_buffer();
}


