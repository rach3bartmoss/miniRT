/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_vectors_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 20:59:08 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/11 10:03:17 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	init_rays(t_ray_table *ray_table)
{
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

/// @brief Solve ndc/screen values depending on the flag
/// @param win Struct to have access to window width and height
/// @param flag 'w' to calculate ndc based on width 'h' for height
/// @param xy_index uses 'x' index for width, 'y' for height
/// @return return a float ndc value on sucess, -1.0f on failure
float	solve_screen_uv(t_window *win, t_cam_basis *cam_basis,
	char flag, int xy_index)
{
	float	ndc;
	float	screen_axis;
	float	res_uv;

	if (flag == 'w')
	{
		ndc = (xy_index + 0.5f) / (float)win->width;
		screen_axis = 2.0f * ndc - 1.0f;
		res_uv = screen_axis * cam_basis->half_width;
		return (res_uv);
	}
	if (flag == 'h')
	{
		ndc = (xy_index + 0.5f) / (float)win->height;
		screen_axis = 1.0f - 2.0f * ndc;
		res_uv = screen_axis * cam_basis->half_height;
		return (res_uv);
	}
	return (-1.0f);
}

int	fill_vectors_helper(t_cam_basis *cam_basis, t_screen_ndc_ctx *ndc_ctx,
	t_ray_table *ray_table)
{
	float	dir[3];

	dir[0] = cam_basis->forward[0] + ndc_ctx->u * cam_basis->right[0]
		+ ndc_ctx->v * cam_basis->up[0];
	dir[1] = cam_basis->forward[1] + ndc_ctx->u * cam_basis->right[1]
		+ ndc_ctx->v * cam_basis->up[1];
	dir[2] = cam_basis->forward[2] + ndc_ctx->u * cam_basis->right[2]
		+ ndc_ctx->v * cam_basis->up[2];
	normalize(dir, dir);
	if (ndc_ctx->index >= ray_table->total_rays)
	{
		printf("Index %d out of bounds (total %d)\n",
			ndc_ctx->index, ray_table->total_rays);
		return (0);
	}
	ray_table->vectors_x[ndc_ctx->index] = dir[0];
	ray_table->vectors_y[ndc_ctx->index] = dir[1];
	ray_table->vectors_z[ndc_ctx->index] = dir[2];
	return (1);
}

//exit call can lead to leaks?
int	calc_vectors(t_ray_table *ray_table, t_cam_basis *cam_basis, t_window *win)
{
	t_screen_ndc_ctx	ndc_ctx;

	ndc_ctx.y_index = 0;
	while (ndc_ctx.y_index < win->height)
	{
		ndc_ctx.x_index = 0;
		while (ndc_ctx.x_index < win->width)
		{
			ndc_ctx.index = ndc_ctx.y_index * win->width + ndc_ctx.x_index;
			ndc_ctx.u = solve_screen_uv(win, cam_basis, 'w', ndc_ctx.x_index);
			ndc_ctx.v = solve_screen_uv(win, cam_basis, 'h', ndc_ctx.y_index);
			if (fill_vectors_helper(cam_basis, &ndc_ctx, ray_table) == 0)
				exit (1);
			ndc_ctx.x_index++;
		}
		ndc_ctx.y_index++;
	}
	return (1);
}
