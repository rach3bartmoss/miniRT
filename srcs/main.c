/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:36:30 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/20 09:39:19 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	pre_runtime_check(char **argv, int argc)
{
	if (argc != 2)
	{
		write(STDERR_FILENO, "Error: Usage: ./minirt <filename>.rt\n", 38);
		return (1);
	}
	if (check_filename(argv[1]))
		printf("FILENAME IS VALID!\n");
	else
	{
		printf("File '%s' is not valid!\n", argv[1]);
		return (1);
	}
	return (0);
}

int	init_app(t_app *app, char **argv, t_window *win)
{
	int	fd;

	fd = file_management(argv[1]);
	if (fd == -1)
		return (1);
	ft_memset(app->scene, 0, sizeof(t_scene));
	ft_memset(app->win, 0, sizeof(t_window));
	if (parse_file(app->scene, fd) != 1)
	{
		cleanup_all(app->scene);
		return (1);
	}
	app->win->mlx = mlx_init();
	app->win->width = 1280;
	app->win->height = 720;
	app->win->aspect_ratio = (float)win->width / (float)win->height;
	app->win->win = mlx_new_window(win->mlx, win->width, win->height, "miniRT");
	app->win->img = mlx_new_image(win->mlx, win->width, win->height);
	app->win->addr = mlx_get_data_addr(win->img, &win->bpp, &win->line_length,
			&win->endian);
	app->click_lock = 0;
	app->n_textures = 0;
	app->preset_list = NULL;
	app->textures = NULL;
	return (0);
}

void	clean_program(t_app *app)
{
	if (app->scene)
		cleanup_all(app->scene);
	if (app->ray_table->vectors_x)
		free(app->ray_table->vectors_x);
	if (app->ray_table->vectors_y)
		free(app->ray_table->vectors_y);
	if (app->ray_table->vectors_z)
		free(app->ray_table->vectors_z);
	if (app->ray_table->hit_record)
		free (app->ray_table->hit_record);
	if (app->textures)
		clean_textures_list(app ,app->textures);
	if (app->preset_list)
		clean_preset_list(app->preset_list);
	if (app->win->win)
		mlx_destroy_window(app->win->mlx, app->win->win);
	if (app->win->img)
		mlx_destroy_image(app->win->mlx, app->win->img);
	if (app->win->mlx)
	{
		mlx_destroy_display(app->win->mlx);
		free(app->win->mlx);
	}
}

int	run_program(t_app *app)
{
	ft_memset(app->ray_table, 0, sizeof(t_ray_table));
	create_rays(app->scene->camera, app->win, app->ray_table);
	if (!app->ray_table->hit_record)
		app->ray_table->hit_record = malloc(sizeof(t_hit)
				* app->ray_table->total_rays);
	if (!app->ray_table->hit_record)
	{
		clean_program(app);
		return (1);
	}
	init_hit_record(app->ray_table);
	render_objects(app);
	mlx_put_image_to_window(app->win->mlx, app->win->win, app->win->img, 0, 0);
	start_multithread_render(app);
	mlx_put_image_to_window(app->win->mlx, app->win->win, app->win->img, 0, 0);
	return (0);
}

int	main(int argc, char **argv)
{
	t_app		app;
	t_scene		scene;
	t_window	win;
	t_ray_table	ray_table;

	if (pre_runtime_check(argv, argc) == 1)
		return (1);
	app.scene = &scene;
	app.win = &win;
	app.ray_table = &ray_table;
	if (init_app(&app, argv, &win) == 1)
		return (1);
	if (run_program(&app) == 1)
		return (1);
	mlx_hook(app.win->win, 17, 0, close_window, &app);
	mlx_hook(app.win->win, 2, 1L << 0, key_press, &app);
	mlx_mouse_hook(app.win->win, mouse_click_handler, &app);
	mlx_loop(app.win->mlx);
	return (0);
}
