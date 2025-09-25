/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_intersection_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:47:34 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/25 16:13:03 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	render_plane(t_ray_table *ray_table, t_scene *scene, t_window *win)
{
	if (!ray_table->hit_record)
		ray_table->hit_record = malloc(sizeof(t_hit) * ray_table->total_rays);
	
}