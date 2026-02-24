/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 13:59:07 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/30 23:20:05 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	render_loop_helper(t_render_ctx	*render, t_window *win,
	t_scene *scene)
{
	apply_ambient_light(scene, render->rec, render);
	if (apply_diffuse_and_shadow(render, scene, win) == 1)
	{
		render->color = rgb3_to_hex(render->out_shade);
		mrt_put_pixel(win, render->x, render->y, render->color);
	}
	else
	{
		render->color = rgb3_to_hex(render->a_shade);
		mrt_put_pixel(win, render->x, render->y, render->color);
	}
}

//mlx_pixel_put(win->mlx, win->win, x, y, 0x000000);
void	render_loop(t_app *app)
{
	t_render_ctx	render;

	ray_sphere_intersect(app->ray_table, app->scene);
	ray_plane_intersect(app->ray_table, app->scene);
	ray_cylinder_intersection(app->ray_table, app->scene);
	render.y = 0;
	while (render.y < app->win->height)
	{
		render.x = 0;
		while (render.x < app->win->width)
		{
			render.i = render.y * app->win->width + render.x;
			render.rec = &app->ray_table->hit_record[render.i];
			if (render.rec->hit)
			{
				render_loop_helper(&render, app->win, app->scene);
			}
			else
				mrt_put_pixel(app->win, render.x, render.y, 0x000000);
			render.x++;
		}
		render.y++;
	}
}
