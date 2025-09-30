/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 20:58:14 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/30 23:47:37 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//mlx_pixel_put(win->mlx, win->win, x, y, 0x000000);
void	render_loop(t_ray_table *ray_table, t_window *win, t_scene *scene)
{
	t_render_ctx	render;

	render.y = 0;
	while (render.y < win->height)
	{
		render.x = 0;
		while (render.x < win->width)
		{
			render.i = render.y * win->width + render.x;
			render.rec = &ray_table->hit_record[render.i];
			if (render.rec->hit)
			{
				//posible pipeline ambient light > shadows / diffuse light term
				apply_ambient_light(scene, render.rec, &render);
				float	w = diffuse_and_shadow_algo(ray_table, scene, &render);
				if (w > 0.0f)
				{
					render.c_obj_term[0] = render.rec->color[0] / 255.0f;
					render.c_obj_term[1] = render.rec->color[1] / 255.0f;
					render.c_obj_term[2] = render.rec->color[2] / 255.0f;

					render.c_light_term[0] = scene->light->light_rgb[0] / 255.0f;
					render.c_light_term[1] = scene->light->light_rgb[1] / 255.0f;
					render.c_light_term[2] = scene->light->light_rgb[2] / 255.0f;

					for (int c = 0; c < 3; ++c)
					{
						float	amb_chan = render.a_shade[c] / 255.0f;
						// add diffuse contribution
						float	chan = amb_chan + w * render.c_light_term[c] * render.c_obj_term[c];
						chan = fminf(chan, 1.0f);           // clamp
						render.a_shade[c] = (int)(chan * 255.0f);
					}
				}

				
				render.color = rgb3_to_hex(render.a_shade);
				mrt_put_pixel(win, render.x, render.y, render.color);
			}
			else
				mrt_put_pixel(win, render.x, render.y, 0x000000);
			render.x++;
		}
		render.y++;
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
