/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_paraboloid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:15:58 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/29 23:22:05 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	fill_paraboloid_prep(t_scene *scene)
{
	size_t	oldsize;

	oldsize = sizeof(t_paraboloid *) * scene->paraboloid_capacity;
	scene->paraboloid_capacity *= 2;
	scene->paraboloid = ft_realloc(scene->paraboloid, oldsize,
			sizeof(t_paraboloid *) * scene->paraboloid_capacity);
	if (!scene->paraboloid)
	{
		printf("miniRT: Paraboloid realloc failed.\n");
		return (0);
	}
	return (1);
}

//pa  <vertex>  <axis>[-1.0, 1.0]  <k>[0.5, 2.0]  <height>  <color>
int	fill_paraboloid_helper(t_paraboloid *new, char *line)
{
	char	*token;
	int		token_count;
	int		rc;

	rc = 1;
	token = ft_strtok(line, " \t\v\f\r\n");
	token_count = 0;
	while (token != NULL && token_count < 6)
	{
		if (token_count == 1)
			rc = parse_coordinates(token, new->center);
		if (token_count == 2 && rc == 1)
			rc = parse_coordinates_vector(token, new->axis);
		if (token_count == 3 && rc == 1)
			rc = parse_steepness(token, &new->k, PARABOLOID);
		if (token_count == 4 && rc == 1)
			rc = parse_height(token, &new->height);
		if (token_count == 5 && rc == 1)
			rc = parse_rgb(token, new->rgb, PARABOLOID);
		if (rc != 1)
			return (0);
		token_count++;
		token = ft_strtok(NULL, " \t\v\f\r\n");
	}
	return (1);
}

//pa  <center/vertex>  <axis>  <k>  <height>  <color>
int	fill_paraboloid(t_scene *scene, char *line)
{
	t_paraboloid	*new_obj;
	char			*parse_line;
	int				count;

	count = set_and_get_occ(-1, PARABOLOID);
	if (count >= scene->paraboloid_capacity)
		if (!fill_paraboloid_prep(scene))
			return (0);
	new_obj = malloc(sizeof(t_paraboloid));
	if (!new_obj)
		return (printf("miniRT: New paraboloid malloc failed\n"), 0);
	ft_memset(new_obj, 0, sizeof(t_paraboloid));
	parse_line = ft_strdup(line);
	new_obj->id = ft_strdup("pa");
	if (fill_paraboloid_helper(new_obj, parse_line) != 1)
	{
		return (handle_default_error((void **)&new_obj, parse_line,
				new_obj->id));
	}
	scene->paraboloid[count] = new_obj;
	set_and_get_occ(count + 1, PARABOLOID);
	free (parse_line);
	return (1);
}
