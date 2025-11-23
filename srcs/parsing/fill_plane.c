/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_plane.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 09:15:54 by dopereir          #+#    #+#             */
/*   Updated: 2025/11/16 21:35:29 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	fill_plane_helper(t_plane *new_plane, char *parse_line)
{
	char	*token;
	int		token_count;
	int		rc;

	rc = 1;
	token = ft_strtok(parse_line, " \t\v\f\r\n");
	token_count = 0;
	while (token != NULL && token_count < 4)
	{
		if (token_count == 1)
			rc = parse_coordinates(token, new_plane->pl_xyz);
		if (token_count == 2 && rc == 1)
			rc = parse_coordinates_vector(token, new_plane->pl_vector_xyz);
		if (token_count == 3 && rc == 1)
			rc = parse_rgb(token, new_plane->pl_rgb, PLANE);
		if (rc != 1)
			return (0);
		token_count++;
		token = ft_strtok(NULL, " \t\v\f\r\n");
	}
	return (1);
}

int	fill_plane_prep(t_scene *scene)
{
	size_t	oldsize;

	oldsize = sizeof(t_plane *) * scene->plane_capacity;
	scene->plane_capacity *= 2;
	scene->plane = ft_realloc(scene->plane, oldsize, sizeof(t_plane *)
			* scene->plane_capacity);
	if (!scene->plane)
	{
		printf("miniRT: Plane realloc failed.\n");
		return (0);
	}
	return (1);
}

int	fill_plane(t_scene *scene, char *line)
{
	t_plane		*new_plane;
	char		*parse_line;
	int			count;

	count = set_and_get_occ(-1, PLANE);
	if (count >= scene->plane_capacity)
	{
		if (!fill_plane_prep(scene))
			return (0);
	}
	new_plane = malloc(sizeof(t_plane));
	if (!new_plane)
		return (printf("New plane malloc failed\n"), 0);
	ft_memset(new_plane, 0, sizeof(t_plane));
	parse_line = ft_strdup(line);
	new_plane->id = ft_strdup("pl");
	if (fill_plane_helper(new_plane, parse_line) != 1)
		return (handle_default_error((void **)&new_plane,
				parse_line, new_plane->id));
	scene->plane[count] = new_plane;
	set_and_get_occ(count + 1, PLANE);
	free (parse_line);
	return (1);
}
