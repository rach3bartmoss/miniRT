/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 21:41:58 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/15 21:45:10 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief Free a array of string created by ft_split()
/// @param parts The target array.
void	free_split(char **parts)
{
	size_t	i;

	i = 0;
	if (!parts)
		return;
	while (parts[i])
	{
		free(parts[i]);
		i++;
	}
	free(parts);
}
