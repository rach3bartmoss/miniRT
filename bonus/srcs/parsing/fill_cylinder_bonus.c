/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cylinder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 13:29:31 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 21:24:59 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

int	validate_height_str(char *height_str)
{
	int	i;

	i = 0;
	if (!height_str)
		return (0);
	while (height_str[i] == '+' || height_str[i] == '-')
		i++;
	if (check_dots(height_str, i) == 0)
		return (0);
	while (height_str[i])
	{
		if (height_str[i] == '.')
		{
			i++;
			continue ;
		}
		if (ft_isdigit(height_str[i]) != 1)
		{
			printf("miniRT: invalid height format: '%s'\n", height_str);
			return (0);
		}
		i++;
	}
	return (1);
}

int	parse_height(char *height_str, double *height_target)
{
	char	*endptr;
	double	res;
	int		sign;

	sign = 1;
	if (!validate_height_str(height_str))
		return (0);
	sign = sign_handler(&height_str);
	res = ft_strtof(height_str, &endptr);
	if (*endptr != '\0')
	{
		printf("miniRT: height value conversion failed: %s\n", endptr);
		return (0);
	}
	res *= sign;
	if (res <= 0)
	{
		printf("miniRT: height must be greater than 0: '%f'\n", res);
		return (0);
	}
	*height_target = res;
	return (1);
}

int	fill_cylinder_helper(t_cylinder *new_cylinder, char *parse_line)
{
	char	*token;
	int		token_count;
	int		rc;

	rc = 1;
	token = ft_strtok(parse_line, " \t\v\f\r\n");
	token_count = 0;
	while (token != NULL && token_count < 6)
	{
		if (token_count == 1)
			rc = parse_coordinates(token, new_cylinder->cy_xyz);
		if (token_count == 2 && rc == 1)
			rc = parse_coordinates_vector(token, new_cylinder->cy_vector_xyz);
		if (token_count == 3 && rc == 1)
			rc = parse_diameter(token, &new_cylinder->cy_diameter, CYLINDER);
		if (token_count == 4 && rc == 1)
			rc = parse_height(token, &new_cylinder->cy_height);
		if (token_count == 5 && rc == 1)
			rc = parse_rgb(token, new_cylinder->cy_rgb, CYLINDER);
		if (rc != 1)
			return (0);
		token_count++;
		token = ft_strtok(NULL, " \t\v\f\r\n");
	}
	return (1);
}

static int	fill_cylinder_prep(t_scene *scene)
{
	size_t	oldsize;

	oldsize = sizeof(t_cylinder *) * scene->cylinder_capacity;
	scene->cylinder_capacity *= 2;
	scene->cylinder = ft_realloc(scene->cylinder, oldsize,
			sizeof(t_cylinder *) * scene->cylinder_capacity);
	if (!scene->cylinder)
	{
		printf("miniRT: cylinder ft_realloc failed.\n");
		return (0);
	}
	return (1);
}

int	fill_cylinder(t_scene *scene, char *line)
{
	t_cylinder	*new_cylinder;
	char		*parse_line;
	int			count;

	count = set_and_get_occ(-1, CYLINDER);
	if (count >= scene->cylinder_capacity)
	{
		if (!fill_cylinder_prep(scene))
			return (0);
	}
	new_cylinder = malloc(sizeof(t_cylinder));
	if (!new_cylinder)
		return (printf("New cylinder malloc failed\n"), 0);
	ft_memset(new_cylinder, 0, sizeof(t_cylinder));
	parse_line = ft_strdup(line);
	new_cylinder->id = ft_strdup("cy");
	if (fill_cylinder_helper(new_cylinder, parse_line) != 1)
		return (handle_default_error((void **)&new_cylinder,
				parse_line, new_cylinder->id));
	scene->cylinder[count] = new_cylinder;
	set_and_get_occ(count + 1, CYLINDER);
	free(parse_line);
	return (1);
}
