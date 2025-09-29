/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 20:58:14 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/29 22:03:07 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//mlx_pixel_put(win->mlx, win->win, x, y, 0x000000);
void	render_loop(t_ray_table *ray_table, t_window *win, t_scene *scene)
{
	t_hit	*rec;
	int		a_shade[3];
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
				apply_ambient_light(scene, rec, a_shade);
				color = rgb3_to_hex(a_shade);
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
	ray_sphere_intersect(ray_table, scene);
	render_loop(ray_table, win, scene);
	mlx_put_image_to_window(win->mlx, win->win, win->img, 0, 0);
	return (1);
}
