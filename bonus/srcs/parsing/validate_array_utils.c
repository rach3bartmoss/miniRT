/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_array_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 21:52:32 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/18 00:35:11 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	validate_rgb_components(char **components, char *rgb_str)
{
	int	i;

	i = 0;
	if (!components)
	{
		printf("miniRT: parse_rgb: ft_split failed.\n");
		free_split(components);
		return (0);
	}
	while (components[i])
		i++;
	if (i != 3)
	{
		printf("Incorrect RGB format: %s\n", rgb_str);
		return (0);
	}
	return (1);
}

/// @brief Validate coordinates of a target element, note: 
/// @brief xyz_str is freed in the caller.
/// @param coordinates 
/// @param xyz_str 
/// @return 0 on failure, 1 on success.
int	validate_coordinates(char **coordinates, char *xyz_str)
{
	int	i;

	i = 0;
	if (!coordinates)
	{
		printf("miniRT: parse_coordinates: ft_split failed.\n");
		free_split(coordinates);
		return (0);
	}
	while (coordinates[i])
		i++;
	if (i != 3)
	{
		printf("Incorrect coordinates format: %s\n", xyz_str);
		return (0);
	}
	return (1);
}
