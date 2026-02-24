/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 23:47:24 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/30 22:29:35 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_rgb(char *rgb_str, int rgb_target[3], t_type_elem type)
{
	char	**components;
	int		i;

	i = 0;
	components = ft_split(rgb_str, ',');
	if (validate_rgb_components(components, rgb_str) != 1)
	{
		free_split(components);
		return (0);
	}
	while (i < 3)
	{
		if (!components[i])
			return (0);
		rgb_target[i] = ft_atoi(components[i]);
		if (rgb_target[i] < 0 || rgb_target[i] > 255)
		{
			printf("Invalid %s RGB value: '%d' (Range: 0-255)\n",
				get_type_name(type), rgb_target[i]);
			return (free_split(components), 0);
		}
		i++;
	}
	free_split(components);
	return (1);
}

/// @brief Will validate light/bright ratios that must been between
/// @brief 0.0 and 1.0
/// @param ratio_str String with the value to convert.
/// @param light_ratio A pointer to the light/brigth ratio value.
/// @return return 1 on sucess, 0 on failure
int	parse_light_ratio(char *ratio_str, float *light_ratio, t_type_elem type)
{
	char	*endptr;
	float	res;

	res = ft_strtof(ratio_str, &endptr);
	if (*endptr != '\0')
	{
		printf("miniRT: Invalid float format for %s ratio\n",
			get_type_name(type));
		return (0);
	}
	if (res >= 0.0f && res <= 1.0f)
	{
		*light_ratio = res;
		return (1);
	}
	printf("miniRT: %s ratio should be between 0.0 and 1.0\n",
		get_type_name(type));
	return (0);
}

/// @brief Poses as setter and getter function.
/// @brief Contains a static int array with 6 elements, holds occurences values
/// @brief of each information on the scene
/// @param value Value to update
/// @param index Infomation to update
/// @return -1 as value returns the index element value, value != -1 sets
int	set_and_get_occ(int value, t_type_elem index)
{
	static int	code[6] = {0};

	if (value == -1)
		return (code[index]);
	code[index] = value;
	return (code[index]);
}

/// @brief Free two targer string and a scene->info
/// @param scene_elem 
/// @param str1 
/// @param str2 
/// @return 
int	handle_default_error(void **scene_elem, void *str1, void *str2)
{
	if (str1)
		free (str1);
	if (str2)
		free (str2);
	if (scene_elem && *scene_elem)
	{
		free (*scene_elem);
		*scene_elem = NULL;
	}
	return (0);
}

/// @brief Initialize a scene info and validate allocations
/// @param target Scene info
/// @param line 
/// @param parse_line line to be duplicated
/// @param type type of the scene info
/// @return 1 on success, 0 on failure
int	pre_elem_check(void **target, char *line,
	char **parse_line, t_type_elem type)
{
	size_t	elem_size;

	elem_size = get_elem_size(type);
	*parse_line = ft_strdup(line);
	if (!*parse_line)
		return (0);
	*target = malloc(elem_size);
	if (!*target)
	{
		free(*parse_line);
		return (0);
	}
	return (1);
}
