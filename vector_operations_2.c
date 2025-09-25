/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 20:16:18 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/25 13:50:24 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	copy_vectors(float	out[3], float in[3])
{
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
}

void	copy_int_vectors(int out[3], int in[3])
{
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
}
