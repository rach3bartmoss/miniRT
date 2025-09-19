/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 22:27:52 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/18 22:28:08 by dopereir         ###   ########.fr       */
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
