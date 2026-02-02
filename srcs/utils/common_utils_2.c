/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 22:55:26 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/29 22:27:13 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


/// @brief Usually used to validade center/origin XYZ Vector
/// @param xyz_str 
/// @param xyz_target 
/// @return Returns 1 on success 0 on failure
int	parse_coordinates(char *xyz_str, float xyz_target[3])
{
	char	**coordinates;
	int		i;
	char	*endptr;

	i = 0;
	coordinates = ft_split(xyz_str, ',');
	if (validate_coordinates(coordinates, xyz_str) != 1)
	{
		free_split(coordinates);
		return (0);
	}
	while (i < 3)
	{
		if (!coordinates[i])
			return (0);
		xyz_target[i] = ft_strtof(coordinates[i], &endptr);
		if (*endptr != '\0')
		{
			printf("miniRT: Invalid float format for coordinates (3)\n");
			free_split(coordinates);
			return (0);
		}
		i++;
	}
	return (free_split(coordinates), 1);
}

static int	parse_vector_loop(char	*vector_value, float *vector_target, int *i)
{
	char	*endptr;

	if (!vector_value)
		return (0);
	vector_target[*i] = ft_strtof(vector_value, &endptr);
	if (*endptr != '\0')
	{
		printf("miniRT: Invalid float format for coordinates vectors (3)\n");
		return (0);
	}
	if (vector_target[*i] < -1.0 || vector_target[*i] > 1.0)
	{
		printf("miniRT: Vector out-of-range: '%f' (Range: -1.0~1.0)\n",
			vector_target[*i]);
		return (0);
	}
	return (1);
}

/// @brief Usually used for validate axus/direction XYZ vector,
/// @brief that must be in a [-1, 1] range.
/// @param vector_str 
/// @param vector_target 
/// @return 
int	parse_coordinates_vector(char *vector_str, float vector_target[3])
{
	char	**vector_values;
	int		i;

	i = 0;
	vector_values = ft_split(vector_str, ',');
	if (validate_coordinates(vector_values, vector_str) != 1)
	{
		free_split(vector_values);
		return (0);
	}
	while (i < 3)
	{
		if (!parse_vector_loop(vector_values[i], vector_target, &i))
		{
			free_split(vector_values);
			return (0);
		}
		i++;
	}
	free_split(vector_values);
	return (1);
}

int	validate_fov_str(char *fov_str)
{
	int	i;

	i = 0;
	if (!fov_str)
		return (0);
	while (fov_str[i] == '+' || fov_str[i] == '-')
		i++;
	if (check_dots(fov_str, i) == 0)
		return (0);
	while (fov_str[i])
	{
		if (fov_str[i] == '.')
		{
			i++;
			continue ;
		}
		if (ft_isdigit(fov_str[i]) != 1)
		{
			printf("miniRT: invalid FOV format: '%s'\n", fov_str);
			return (0);
		}
		i++;
	}
	return (1);
}

int	parse_fov(char *fov_str, float *fov_target)
{
	float	res;
	char	*endptr;
	int		sign;

	sign = 1;
	if (!validate_fov_str(fov_str))
		return (0);
	sign = sign_handler(&fov_str);
	res = ft_strtof(fov_str, &endptr);
	if (*endptr != '\0')
	{
		printf("miniRT: Fov value conversion failed: %s\n", endptr);
		return (0);
	}
	res *= sign;
	if (res < 0 || res > 180)
	{
		printf("miniRT: FOV out-of-range: '%f' (Range: 0-180)\n", res);
		return (0);
	}
	*fov_target = res;
	return (1);
}
