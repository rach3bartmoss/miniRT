/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 23:30:36 by joao-vri          #+#    #+#             */
/*   Updated: 2026/02/23 23:52:15 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	render_loop_helper(t_render_ctx	*render, t_window *win,
	t_scene *scene)
{
	copy_vectors(render->rec->shading_normal, render->rec->normal);
	apply_object_base_color(render, scene);
	apply_textures_for_hit(render->rec, scene);
	apply_ambient_light(scene, render->rec, render);
	if (apply_diffuse_specular_and_shadow(render, scene, win) == 1)
		render->color = rgb3_to_hex(render->out_shade);
	else
		render->color = rgb3_to_hex(render->a_shade);
	mrt_put_pixel(win, render->x, render->y, render->color);
}

void	start_multithread_render(t_app *app, int n_threads)
{
	pthread_t		threads[192];
	t_thread_data	data[192];
	int				y_start;
	int				i;

	y_start = app->win->height / n_threads;
	i = -1;
	while (++i < n_threads)
	{
		data[i].app = app;
		data[i].y_start = i * y_start;
		if (i == n_threads - 1)
			data[i].y_end = app->win->height;
		else
			data[i].y_end = (i + 1) * y_start;
		pthread_create(&threads[i], NULL, render_thread, &data[i]);
	}
	i = -1;
	while (++i < n_threads)
		pthread_join(threads[i], NULL);
	mlx_put_image_to_window(app->win->mlx, app->win->win, app->win->img, 0, 0);
}

static void	render_thread_helper(t_render_ctx *render, t_ray_table *ray_table,
	t_window *win, t_scene *scene)
{
	render->i = render->y * win->width + render->x;
	render->rec = &ray_table->hit_record[render->i];
	if (render->rec->hit)
		render_loop_helper(render, win, scene);
	else
		mrt_put_pixel(win, render->x, render->y, 0x000000);
	render->x++;
}

//mlx_pixel_put(win->mlx, win->win, x, y, 0x000000);
void	*render_thread(void *data)
{
	t_render_ctx	render;
	t_thread_data	*thread_data;
	t_ray_table		*ray_table;
	t_window		*win;
	t_scene			*scene;

	thread_data = (t_thread_data *)data;
	ray_table = thread_data->app->ray_table;
	win = thread_data->app->win;
	scene = thread_data->app->scene;
	render.y = thread_data->y_start;
	while (render.y < thread_data->y_end)
	{
		render.x = 0;
		while (render.x < win->width)
			render_thread_helper(&render, ray_table, win, scene);
		render.y++;
	}
	return (NULL);
}
