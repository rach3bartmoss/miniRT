/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 13:59:07 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/27 23:55:30 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	apply_object_base_color(t_render_ctx *render, t_scene *scene)
{
	int	type;
	int	idx;

	type = render->rec->object_type;
	idx = render->rec->obj_scene_idx;
	if (type == PLANE && scene->plane[idx]->checkerboard)
		apply_checkerboard_for_plane(render->rec,
			scene->plane[idx], render->rec->color);
	else if (type == SPHERE && scene->sphere[idx]->checkerboard)
		apply_checkboard_for_sphere(render->rec,
			scene->sphere[idx], render->rec->color);
	else if (type == CYLINDER && scene->cylinder[idx]->checkerboard)
		apply_checkerboard_for_cy(render->rec, scene->cylinder[idx],
			render->rec->color);
}

static void	render_loop_helper(t_render_ctx	*render, t_window *win,
	t_scene *scene)
{
	apply_object_base_color(render, scene);
	apply_ambient_light(scene, render->rec, render);
	if (apply_diffuse_specular_and_shadow(render, scene, win) == 1)
		render->color = rgb3_to_hex(render->out_shade);
	else
		render->color = rgb3_to_hex(render->a_shade);
	mrt_put_pixel(win, render->x, render->y, render->color);
}

void	start_multithread_render(t_app *app)
{
	int				n_threads = sysconf(_SC_NPROCESSORS_ONLN);
	pthread_t		threads[n_threads];
	t_thread_data	data[n_threads];
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
		{
			render.i = render.y * win->width + render.x;
			render.rec = &ray_table->hit_record[render.i];
			if (render.rec->hit)
				render_loop_helper(&render, win, scene);
			else
				mrt_put_pixel(win, render.x, render.y, 0x000000);
			render.x++;
		}
		render.y++;
	}
	return (NULL);
}

void	render_objects(t_app *app)
{
	t_hit	*rec;
	int		color;
	int		x;
	int		y;
	int		i;

	ray_sphere_intersect(app->ray_table, app->scene);
	ray_plane_intersect(app->ray_table, app->scene);
	ray_cylinder_intersection(app->ray_table, app->scene);
	y = 0;
	while (y < app->win->height)
	{
		x = 0;
		while (x < app->win->width)
		{
			i = y * app->win->width + x;
			rec = &app->ray_table->hit_record[i];
			if (!rec->hit)
				color = 0x000000;
			else
				color = apply_shade_to_pixel(app, rec);
			mrt_put_pixel(app->win, x, y, color);
			x++;
		}
		y++;
	}
}
