/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_camera.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 23:13:24 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/20 15:12:37 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	fill_camera_helper(t_scene *scene, char *parse_line, int *rc)
{
	char	*token;
	int		token_count;

	*rc = 1;
	token = ft_strtok(parse_line, " \t\v\f\r\n");
	token_count = 0;
	while (token != NULL && token_count < 4)
	{
		if (token_count == 1)
			*rc = parse_coordinates(token, scene->camera->coordinates_xyz);
		if (token_count == 2 && *rc == 1)
			*rc = parse_coordinates_vector(token, scene->camera->vector_xyz);
		if (token_count == 3 && *rc == 1)
			*rc = parse_fov(token, &scene->camera->fov);
		if (*rc != 1)
			return (0);
		token_count++;
		token = ft_strtok(NULL, " \t\v\f\r\n");
	}
	return (1);
}

int	fill_camera(t_scene *scene, char *line)
{
	char	*parse_line;
	int		rc;

	if (set_and_get_occ(GET_OCCURRENCE, CAMERA) > 0)
	{
		printf("miniRT: input file has more than one 'C' camera line\n");
		return (0);
	}
	parse_line = NULL;
	if (!pre_elem_check((void **)&scene->camera, line, &parse_line, CAMERA))
		return (0);
	ft_memset(scene->camera, 0, sizeof(t_camera));
	scene->camera->id = 'C';
	if (fill_camera_helper(scene, parse_line, &rc) != 1)
		return (handle_default_error((void **)&scene->camera, parse_line,
				NULL));
	set_and_get_occ(1, CAMERA);
	free (parse_line);
	return (1);
}
