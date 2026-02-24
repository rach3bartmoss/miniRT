/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_vectors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:52:02 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 21:24:59 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	init_custom_vec3(double target[3], double a, double b, double c)
{
	target[0] = a;
	target[1] = b;
	target[2] = c;
}

/// @brief 
/// @param camera Camera struct with FOV and xyz coordinates.
/// @param win contains screen info
/// @return 
int	create_rays(t_camera *camera, t_window *win, t_ray_table *ray_table)
{
	t_cam_basis	cam_basis;
	double		fov_radian;
	double		world_up[3];
	double		cross_tmp[3];

	init_custom_vec3(world_up, 0, 1, 0);
	init_custom_vec3(cross_tmp, 0, 0, 0);
	fov_radian = camera->fov * (M_PI / 180.0f);
	cam_basis.half_width = tanf(fov_radian / 2.0f);
	cam_basis.half_height = cam_basis.half_width / win->aspect_ratio;
	ray_table->total_rays = win->width * win->height;
	ray_table->half_width = cam_basis.half_width;
	ray_table->half_height = cam_basis.half_height;
	normalize(camera->vector_xyz, cam_basis.forward);
	cross(world_up, cam_basis.forward, cross_tmp);
	normalize(cross_tmp, cam_basis.right);
	cross(cam_basis.forward, cam_basis.right, cam_basis.up);
	init_rays(ray_table);
	calc_vectors(ray_table, &cam_basis, win);
	return (1);
}

/*DEBUG PRINTS:
	printf("half_width: %f\nhalf_height: %f\n",
		cam_basis.half_width, cam_basis.half_height);
	print_array3(cam_basis.forward);
	print_array3(cam_basis.right);
	print_array3(cam_basis.up);
*/
