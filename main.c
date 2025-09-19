/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:36:30 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/20 00:07:45 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	main(int argc, char **argv)
{
	t_scene	scene;
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

	parse_file(&scene, fd);
	printf("\n");
	print_element(scene.ambiance, AMBIENT);
	print_element(scene.camera, CAMERA);
	print_element(scene.light, LIGHT);
	
	print_spheres(&scene);
	
	return (0);
}