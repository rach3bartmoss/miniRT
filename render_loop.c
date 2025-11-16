/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 13:59:07 by dopereir          #+#    #+#             */
/*   Updated: 2025/11/16 14:11:19 by dopereir         ###   ########.fr       */
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
				render_loop_helper(&render, win, scene);
			}
			else
				mrt_put_pixel(win, render.x, render.y, 0x000000);
			render.x++;
		}
		render.y++;
	}
}
