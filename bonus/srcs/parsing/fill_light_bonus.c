/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_light.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 22:42:45 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/09 20:30:17 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

int	fill_light_helper(t_scene *scene, char *parse_line, int *rc)
{
	char	*token;
	int		token_count;

	*rc = 1;
	token = ft_strtok(parse_line, " \t\v\f\r\n");
	token_count = 0;
	while (token != NULL && token_count < 4)
	{
		if (token_count == 1)
			*rc = parse_coordinates(token, scene->light->light_xyz);
		if (token_count == 2 && *rc == 1)
			*rc = parse_light_ratio(token, &scene->light->bright_ratio, LIGHT);
		if (token_count == 3 && *rc == 1)
			*rc = parse_rgb(token, scene->light->light_rgb, LIGHT);
		if (*rc != 1)
			return (0);
		token_count++;
		token = ft_strtok(NULL, " \t\v\f\r\n");
	}
	return (1);
}

int	fill_light(t_scene *scene, char *line)
{
	char	*parse_line;
	int		rc;

	if (set_and_get_occ(GET_OCCURRENCE, LIGHT) > 0)
	{
		printf("miniRT: input file has more than one 'L' Light line\n");
		return (0);
	}
	parse_line = NULL;
	if (!pre_elem_check((void **)&scene->light, line, &parse_line, LIGHT))
		return (0);
	ft_memset(scene->light, 0, sizeof(t_light));
	scene->light->id = 'L';
	if (fill_light_helper(scene, parse_line, &rc) != 1)
		return (handle_default_error((void **)&scene->camera,
				parse_line, NULL));
	set_and_get_occ(1, LIGHT);
	free (parse_line);
	return (1);
}
