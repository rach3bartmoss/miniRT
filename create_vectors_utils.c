/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_vectors_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 20:59:08 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/25 12:18:38 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief Works like a procedural math API, avoids heap allocations
/// @param a_xyz the first float array
/// @param b_xyz the second float array
/// @param target_xyz the resulting cross product of a_xyz and b_xyz;
/// @return 
int	cross(float *a_xyz, float *b_xyz, float *target_xyz)
{
	target_xyz[0] = (a_xyz[1] * b_xyz[2]) - (a_xyz[2] * b_xyz[1]);
	target_xyz[1] = (a_xyz[2] * b_xyz[0]) - (a_xyz[0] * b_xyz[2]);
	target_xyz[2] = (a_xyz[0] * b_xyz[1]) - (a_xyz[1] * b_xyz[0]);
	return (1);
}

int	init_rays(t_ray_table *ray_table)
{
	printf("Allocating %d floats for vectors\n", ray_table->total_rays);
	//
	ray_table->vectors_x = malloc(sizeof(float) * ray_table->total_rays);
	if (!ray_table->vectors_x)
	{
		printf("miniRT: Vectors X failed (malloc)\n");
		return (0);
	}
	ray_table->vectors_y = malloc(sizeof(float) * ray_table->total_rays);
	if (!ray_table->vectors_y)
	{
		printf("miniRT: Vectors Y failed (malloc)\n");
		free (ray_table->vectors_x);
		return (0);
	}
	ray_table->vectors_z = malloc(sizeof(float) * ray_table->total_rays);
	if (!ray_table->vectors_z)
	{
		printf("miniRT: Vectors Z failed (malloc)\n");
		free (ray_table->vectors_x);
		free (ray_table->vectors_y);
		return (0);
	}
	return (1);
}

int	calc_vectors(t_ray_table *ray_table, t_cam_basis *cam_basis, t_window *win)
{
	int	x;
	int	y;
	float	ndc_x;
	float	ndc_y;
	float	screen_x;
	float	screen_y;
	float	u;
	float	v;

	y = 0;
	while (y < win->height)
	{
		x = 0;
		while (x < win->width)
		{
			ndc_x = (x + 0.5f) / (float)win->width; // formula:	NDCx = pixelX + 0.5 / ImageWidth
			ndc_y = (y + 0.5f) / (float)win->height;//			NDCy = pixelY + 0.5 / ImageHeight
			
			screen_x = 2.0f * ndc_x - 1.0f; // formula:	PixelScreenX = 2 * NDCx - 1
			screen_y = 1.0f - 2.0f * ndc_y; //			PixelScreenY = 1 - 2 * NDCy
			
			u = screen_x * cam_basis->half_width;
			v = screen_y * cam_basis->half_height;
			float	dir_tmp[3] = {
				cam_basis->forward[0] + u * cam_basis->right[0] + v * cam_basis->up[0],
				cam_basis->forward[1] + u * cam_basis->right[1] + v * cam_basis->up[1],
				cam_basis->forward[2] + u * cam_basis->right[2] + v * cam_basis->up[2]
			};

			float	dir[3];
			normalize(dir_tmp, dir);

			int	i = y * win->width + x;
			if (i >= ray_table->total_rays)
			{
				printf("Index %d out of bounds (total %d)\n", i, ray_table->total_rays);
				exit(1);
			}
			ray_table->vectors_x[i] = dir[0];
			ray_table->vectors_y[i] = dir[1];
			ray_table->vectors_z[i] = dir[2];
			x++;
		}
		y++;
	}

	printf("(500,100) x vector: %f\n", ray_table->vectors_x[80500]);
	printf("(500,100) y vector: %f\n", ray_table->vectors_y[80500]);
	printf("(500,100) z vector: %f\n", ray_table->vectors_z[80500]);
	return (1);
}
