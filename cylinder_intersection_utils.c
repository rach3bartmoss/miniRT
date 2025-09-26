/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersection_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 10:24:03 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/26 20:38:52 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/* We want to find B (base center)
C = Center of  the cylinder
h = half-height
A = Vector XYZ normalized (Normal)

(BASE CENTER EQUATION)
B = C - hA
B = (0, 0, 40) - (21.12 / 2) * (0, 0, 1)

To find T (top center) is the same logic, but we sum
C = Center of  the cylinder
h = half-height
A = Vector XYZ normalized (Normal)

T = C + hA
B = (0, 0, 40) + (21.12 / 2) * (0, 0, 1)
*/
int	def_cylinder_geometry(t_cylinder *curr_cy)
{
	float	base_center[3];
	float	top_center[3];
	float	hA[3];
	float	half_height;

	half_height = curr_cy->cy_height / 2;
	scale(hA, curr_cy->cy_vector_xyz, half_height);
	sub(base_center, curr_cy->cy_xyz, hA);

	add(top_center, curr_cy->cy_xyz, hA);
}