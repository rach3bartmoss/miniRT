/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils_4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 22:33:42 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/06 21:15:23 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	validate_steep_str(char *diameter_str)
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
			printf("miniRT: invalid steep format: '%s'\n", diameter_str);
			return (0);
		}
		i++;
	}
	return (1);
}

int	parse_steepness(char *steep_str, float *steep_target, t_type_elem type)
{
	char	*endptr;
	float	res;
	int		sign;

	sign = 1;
	if (!validate_steep_str(steep_str))
		return (0);
	sign = sign_handler(&steep_str);
	res = ft_strtof(steep_str, &endptr);
	if (*endptr != '\0')
	{
		printf("miniRT: steepness(K) value conversion failed: %s\n", endptr);
		return (0);
	}
	res *= sign;
	if (res <= 0.4f || res > 2.0f)
	{
		printf("miniRT: '%s' steepnees must be in [0.5 - 2.0 range]: '%f'\n",
			get_type_name(type), res);
		return (0);
	}
	*steep_target = res;
	return (1);
}

void	clean_paraboloid_and_cylinder(t_scene *scene)
{
	int	count;
	int	i;

	count = set_and_get_occ(-1, PARABOLOID);
	i = 0;
	while (i < count)
	{
		free(scene->paraboloid[i]->id);
		free(scene->paraboloid[i]);
		i++;
	}
	free(scene->paraboloid);
	count = set_and_get_occ(-1, CYLINDER);
	i = 0;
	while (i < count)
	{
		free(scene->cylinder[i]->id);
		free(scene->cylinder[i]);
		i++;
	}
	free(scene->cylinder);
}
