/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_intersection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 22:09:28 by dopereir          #+#    #+#             */
/*   Updated: 2025/11/16 22:11:38 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	ray_plane_intersect(t_ray_table *ray_table, t_scene *scene)
{
	t_pl_ctx	pl_ctx;

	pl_ctx.i = 0;
	while (pl_ctx.i < ray_table->total_rays)
	{
		pl_ctx.d[0] = ray_table->vectors_x[pl_ctx.i];
		pl_ctx.d[1] = ray_table->vectors_y[pl_ctx.i];
		pl_ctx.d[2] = ray_table->vectors_z[pl_ctx.i];
		pl_ctx.rec = &ray_table->hit_record[pl_ctx.i];
		pl_ctx.s = 0;
		while (pl_ctx.s < scene->plane_capacity && scene->plane[pl_ctx.s])
		{
			pl_ctx.curr_pl = scene->plane[pl_ctx.s];
			solve_pl_formula(scene, &pl_ctx, 1);
			pl_ctx.s++;
		}
		pl_ctx.i++;
	}
	return (1);
}

//mlx_pixel_put(win->mlx, win->win, x, y, 0x000000);
static void	render_plane_loop(t_ray_table *ray_table, t_window *win)
{
	t_hit	*rec;
	int		color;
	int		x;
	int		y;
	int		i;

	y = 0;
	while (y < win->height)
	{
		x = 0;
		while (x < win->width)
		{
			i = y * win->width + x;
			rec = &ray_table->hit_record[i];
			if (rec->hit)
			{
				color = rgb3_to_hex(rec->color);
				mrt_put_pixel(win, x, y, color);
			}
			else
				mrt_put_pixel(win, x, y, 0x000000);
			x++;
		}
		y++;
	}
}

int	render_plane(t_ray_table *ray_table, t_scene *scene, t_window *win)
{
	ray_plane_intersect(ray_table, scene);
	render_plane_loop(ray_table, win);
	mlx_put_image_to_window(win->mlx, win->win, win->img, 0, 0);
	return (1);
}
