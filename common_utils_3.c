/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 22:27:52 by dopereir          #+#    #+#             */
/*   Updated: 2025/11/16 21:40:42 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief Iterate a string representative of a number to get the effective
/// @brief signal. Note: the string pointer used as parameter will be updated.
/// @param target_str The string pointer
/// @return return the sign, either 1 or -1
int	sign_handler(char **target_str)
{
	int	sign;

	sign = 1;
	while (**target_str == '+' || **target_str == '-')
	{
		if (**target_str == '-')
			sign *= -1;
		(*target_str)++;
	}
	return (sign);
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
