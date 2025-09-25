/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 20:58:14 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/25 14:46:26 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//mlx_pixel_put(win->mlx, win->win, x, y, 0x000000);
void	render_sphere_loop(t_ray_table *ray_table, t_window *win)
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

// p(t) = e + t * d
// e = ray origin
//d = ray direction
int	render_sphere(t_ray_table *ray_table, t_scene *scene, t_window *win)
{
	if (!ray_table->hit_record)
		ray_table->hit_record = malloc(sizeof(t_hit) * ray_table->total_rays);
	init_hit_record(ray_table);
	ray_sphere_intersect(ray_table, scene);
	render_sphere_loop(ray_table, win);
	mlx_put_image_to_window(win->mlx, win->win, win->img, 0, 0);
	return (1);
}
