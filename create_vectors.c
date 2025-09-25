/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_vectors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:52:02 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/24 22:21:37 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief 
/// @param camera Camera struct with FOV and xyz coordinates.
/// @param win contains screen info
/// @return 
int	create_rays(t_camera *camera, t_window *win, t_ray_table *ray_table)
{
	t_cam_basis	cam_basis;
	float		fov_radian;
	float		world_up[3] = {0, 1, 0};
	float		cross_tmp[3] = {0, 0, 0};

	//printf("***width: %d\nheight: %d\nratio: %f\n", win->width, win->height, win->aspect_ratio);
	fov_radian = camera->fov * (M_PI / 180.0f);
	cam_basis.half_width = tanf(fov_radian / 2.0f);
	cam_basis.half_height = cam_basis.half_width / win->aspect_ratio;

	printf("half_width: %f\nhalf_height: %f\n", cam_basis.half_width, cam_basis.half_height);
	

	ray_table->total_rays = win->width * win->height;
	ray_table->half_width = cam_basis.half_width;
	ray_table->half_height = cam_basis.half_height;

	normalize(camera->vector_xyz, cam_basis.forward);
	cross(world_up, cam_basis.forward, cross_tmp);
	normalize(cross_tmp, cam_basis.right);
	cross(cam_basis.forward, cam_basis.right, cam_basis.up);

	init_rays(ray_table);

	print_array3(cam_basis.forward);
	print_array3(cam_basis.right);
	print_array3(cam_basis.up);

	calc_vectors(ray_table, &cam_basis, win);
	
	
	return (1);
}
