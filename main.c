/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ginfranc <ginfranc@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:36:30 by dopereir          #+#    #+#             */
/*   Updated: 2025/11/22 13:55:55 by ginfranc         ###   ########.fr       */
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
	ft_memset(app->scene, 0, sizeof(t_scene));
	if (parse_file(app->scene, argv[1]) != 1)
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

	ft_memset(app.scene, 0, sizeof(t_scene));
	if (parse_file(app.scene, argv[1]) != 1)
	{
		cleanup_all(app.scene);
		return (0);
	}
	printf("\n");
	print_element(app.scene->ambiance, AMBIENT);
	print_element(app.scene->camera, CAMERA);
	print_element(app.scene->light, LIGHT);
	print_spheres(app.scene);
	print_planes(app.scene);
	print_cylinders(app.scene);
	app.win->mlx = mlx_init();
	app.win->width = 1280;
	app.win->height = 720;
	app.win->aspect_ratio = (float)win.width / (float)win.height;
	app.win->win = mlx_new_window(win.mlx, win.width, win.height, "miniRT");
	app.win->img = mlx_new_image(win.mlx, win.width, win.height);
	app.win->addr = mlx_get_data_addr(win.img, &win.bpp, &win.line_length, &win.endian);
	
	//if (init_app(&app, argv, &win) == 1)
		//return (1);
	ft_memset(app.ray_table, 0, sizeof(t_ray_table));
	create_rays(app.scene->camera, app.win, app.ray_table);

	if (!app.ray_table->hit_record)
		app.ray_table->hit_record = malloc(sizeof(t_hit) * app.ray_table->total_rays);
	init_hit_record(app.ray_table);
	
	render_plane(app.ray_table, app.scene, app.win);
	render_cylinder(app.ray_table, app.scene, app.win);
	render_sphere(app.ray_table, app.scene, app.win);
	
	render_loop(app.ray_table, app.win, app.scene);
	mlx_put_image_to_window(app.win->mlx, app.win->win, app.win->img, 0, 0);

	mlx_hook(app.win->win, 17, 0, close_window, &app);
	mlx_hook(app.win->win, 2, 1L<<0, key_press, &app);
	mlx_loop(app.win->mlx);

	return (0);
}
