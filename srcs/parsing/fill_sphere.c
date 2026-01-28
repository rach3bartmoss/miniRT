/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_sphere.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:24:59 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/26 22:08:41 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	fill_sphere_helper(t_sphere *new_sphere, char *parse_line)
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
			rc = parse_coordinates(token, new_sphere->sp_center_xyz);
		if (token_count == 2 && rc == 1)
			rc = parse_diameter(token, &new_sphere->sp_diameter, SPHERE);
		if (token_count == 3 && rc == 1)
			rc = parse_rgb(token, new_sphere->sp_rgb, SPHERE);
		if (rc != 1)
			return (0);
		token_count++;
		token = ft_strtok(NULL, " \t\v\f\r\n");
	}
	return (1);
}

int	fill_sphere_prep(t_scene *scene)
{
	size_t	oldsize;

	oldsize = sizeof(t_sphere *) * scene->sphere_capacity;
	scene->sphere_capacity *= 2;
	scene->sphere = ft_realloc(scene->sphere, oldsize, sizeof(t_sphere *)
			* scene->sphere_capacity);
	if (!scene->sphere)
	{
		printf("miniRT: Sphere realloc failed.\n");
		return (0);
	}
	return (1);
}

int	fill_sphere(t_scene *scene, char *line)
{
	int			count;
	t_sphere	*new_sphere;
	char		*parse_line;

	count = set_and_get_occ(-1, SPHERE);
	if (count >= scene->sphere_capacity)
	{
		if (!fill_sphere_prep(scene))
			return (0);
	}
	new_sphere = malloc(sizeof(t_sphere));
	if (!new_sphere)
		return (printf("New sphere malloc failed\n"), 0);
	ft_memset(new_sphere, 0, sizeof(t_sphere));
	parse_line = ft_strdup(line);
	new_sphere->id = ft_strdup("sp");
	if (fill_sphere_helper(new_sphere, parse_line) != 1)
		return (handle_default_error((void **)&new_sphere,
				parse_line, new_sphere->id));
	scene->sphere[count] = new_sphere;
	set_and_get_occ(count + 1, SPHERE);
	free (parse_line);
	return (1);
}
