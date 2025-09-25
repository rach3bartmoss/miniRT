/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:36:30 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/25 14:26:25 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	main(int argc, char **argv)
{
	t_app		app;
	t_scene		scene;
	t_window	win;
	t_ray_table	ray_table;
	int	fd;

	if (argc != 2)
	{
		write(STDERR_FILENO, "Error: Usage: ./minirt <filename>.rt\n", 38);
		return (1);
	}

	if (check_filename(argv[1]))
		printf("FILENAME IS VALID!\n");
	else
		printf("File '%s' is not valid!\n", argv[1]);
	fd = file_management(argv[1]);

	app.scene = &scene;
	app.win = &win;
	app.ray_table = &ray_table;

	ft_memset(app.scene, 0, sizeof(t_scene));
	if (parse_file(app.scene, fd) != 1)
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
	app.win->width = 800;
	app.win->height = 600;
	app.win->aspect_ratio = (float)win.width / (float)win.height;
	app.win->win = mlx_new_window(win.mlx, win.width, win.height, "miniRT");
	app.win->img = mlx_new_image(win.mlx, win.width, win.height);
	app.win->addr = mlx_get_data_addr(win.img, &win.bpp, &win.line_length, &win.endian);

	ft_memset(app.ray_table, 0, sizeof(t_ray_table));
	create_rays(app.scene->camera, app.win, app.ray_table);

	render_sphere(app.ray_table, app.scene, app.win);

	mlx_hook(app.win->win, 17, 0, close_window, &app);
	mlx_hook(app.win->win, 2, 1L<<0, key_press, &app);
	mlx_loop(app.win->mlx);

	return (0);
}
