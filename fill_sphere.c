/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_sphere.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 22:24:59 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/09 20:31:57 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	check_dots(char *diameter_str, int i)
{
	int	dot_seen;

	dot_seen = 0;
	while (diameter_str[i])
	{
		if (diameter_str[i] == '.')
		{
			if (dot_seen)
			{
				printf("miniRT: invalid diameter fmt (multiple dots): '%s'\n",
					diameter_str);
				return (0);
			}
			dot_seen = 1;
		}
		i++;
	}
	return (1);
}

int	validate_diameter_str(char *diameter_str)
{
	int	i;

	i = 0;
	if (!diameter_str)
		return (0);
	while (diameter_str[i] == '+' || diameter_str[i] == '-')
		i++;
	if (check_dots(diameter_str, i) == 0)
		return (0);
	while (diameter_str[i])
	{
		if (diameter_str[i] == '.')
		{
			i++;
			continue ;
		}
		if (ft_isdigit(diameter_str[i]) != 1)
		{
			printf("miniRT: invalid diameter format: '%s'\n", diameter_str);
			return (0);
		}
		i++;
	}
	return (1);
}

int	parse_diameter(char *diameter_str, float *diameter_target, e_type_elem type)
{
	char	*endptr;
	float	res;
	int		sign;

	sign = 1;
	if (!validate_diameter_str(diameter_str))
		return (0);
	sign = sign_handler(&diameter_str);
	res = ft_strtof(diameter_str, &endptr);
	if (*endptr != '\0')
	{
		printf("miniRT: diameter value conversion failed: %s\n", endptr);
		return (0);
	}
	res *= sign;
	if (res <= 0)
	{
		printf("miniRT: '%s' diameter must be greater than 0: '%f'\n",
			get_type_name(type), res);
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
		scene->sphere = ft_realloc(scene->sphere, oldsize, sizeof(t_sphere *)
				* scene->sphere_capacity);
		if (!scene->sphere)
			return (printf("miniRT: Sphere realloc failed.\n"), 0);
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
