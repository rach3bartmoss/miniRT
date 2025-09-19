/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_sphere.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:24:59 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/20 00:06:42 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	validate_diameter_str(char *diameter_str)
{
	int	i;

	i = 0;
	if (!diameter_str)
		return (0);
	while (diameter_str[i] == '+' || diameter_str[i] == '-')
		i++;
	while (diameter_str[i])
	{
		if (ft_isdigit(diameter_str[i]) != 1)
		{
			printf("miniRT: invalid FOV format: '%s'\n", diameter_str);
			return (0);
		}
		i++;
	}
	return (1);
}

int	parse_sphere_diameter(char *diameter_str, float *diameter_target)
{
	char	*endptr;
	float	res;
	int		sign;

	sign = 1;
	if (!validate_diameter_str(diameter_str))
		return (0);
	sign = sign_handler(&diameter_str);
	res = ft_strtol(diameter_str, &endptr, 10);
	if (*endptr != '\0')
	{
		printf("miniRT: Sphere diameter value conversion failed: %s\n", endptr);
		return (0);
	}
	res *= sign;
	if (res <= 0)
	{
		printf("miniRT: Sphere diameter must be greater than 0: '%f'\n", res);
		return (0);
	}
	*diameter_target = res;
	return (1);
}

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
			rc = parse_sphere_diameter(token, &new_sphere->sp_diameter);
		if (token_count == 3 && rc == 1)
			rc = parse_rgb(token, new_sphere->sp_rgb, SPHERE);
		if (rc != 1)
			return (0);
		token_count++;
		token = ft_strtok(NULL, " \t\v\f\r\n");
	}
	return (1);
}

int	fill_sphere(t_scene *scene, char *line)
{
	int			count;
	t_sphere	*new_sphere;
	char		*parse_line;
	size_t		oldsize;

	count = set_and_get_occ(-1, SPHERE);
	if (count >= scene->sphere_capacity)
	{
		oldsize = sizeof(t_sphere *) * scene->sphere_capacity;
		scene->sphere_capacity *= 2;
		scene->sphere = ft_realloc(scene->sphere, oldsize, sizeof(t_sphere *) * scene->sphere_capacity);
		if (!scene->sphere)
			return (printf("miniRT: Sphere realloc failed.\n"), 0);
	}
	new_sphere = malloc(sizeof(t_sphere));
	if (!new_sphere)
		return (printf("New sphere malloc failed\n"), 0);
	parse_line = ft_strdup(line);
	new_sphere->id = ft_strdup("sp");
	if (fill_sphere_helper(new_sphere, parse_line) != 1)
		return (handle_default_error((void **)&scene->sphere[count],
				parse_line, NULL));
	scene->sphere[count] = new_sphere;
	set_and_get_occ(count + 1, SPHERE);
	return (1);
}
