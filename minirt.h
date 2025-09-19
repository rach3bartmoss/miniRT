/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:36:51 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/19 23:32:48 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H
# define GET_OCCURRENCE -1
# define OBJ_INITIAL_CAPACITY 8

# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include "includes/libft/libft.h"
# include "includes/minilibx-linux/mlx.h"
# include "includes/minilibx-linux/mlx_int.h"

typedef enum e_type
{
	AMBIENT,
	CAMERA,
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER,
}	e_type_elem;

typedef struct s_ambient
{
	char		id; //A
	float		light_ratio;	//range between 0.0 - 1.0
	int			a_rgb[3];			//[0-255]
}	t_ambient;

typedef struct s_camera
{
	char		id; //C
	float		coordinates_xyz[3];			//stores x, y, z coordinates in the order DISCOVER THE RANGE
	float		vector_xyz[3];	//3D orientation vector for each XYZ [0.0~1.0]
	int			fov;			//horizontal field view [0-180]
}	t_camera;

typedef struct s_light
{
	char		id; //L
	float		light_xyz[3];
	float		bright_ratio;		//[0.0~1.0]
	int			light_rgb[3];			//[0-255]
}	t_light;

typedef struct s_sphere
{
	char	*id; //sp
	float	sp_center_xyz[3];
	float	sp_diameter;
	int		sp_rgb[3];
}	t_sphere;

typedef struct s_plane
{
	char	*id; //pl
	float	point_xyz[3];
	float	pl_vector_xyz[3];
	int		pl_rgb[3];
}	t_plane;

typedef struct s_cylinder
{
	char	*id; //cy
	float	cy_center_xyz[3];
	float	cy_vector_xyz[3];
	float	cy_diameter;
	float	cy_height;
	int		cy_rgb[3];
}	t_cylinder;

typedef struct s_scene
{
	t_ambient	*ambiance;
	t_camera	*camera;
	t_light		*light;
	t_sphere	**sphere;
	t_plane		**plane;
	t_cylinder	**cylinder;
	int			sphere_capacity;
	int			plane_capacity;
	int			cylinder_capacity;
}	t_scene;
//clean_utils.c
void	free_split(char **parts);
//common_utils.c
int		parse_rgb(char *rgb_str, int rgb_target[3], e_type_elem type);
int		parse_light_ratio(char *ratio_str, float *light_ratio, e_type_elem type);
int		set_and_get_occ(int value, e_type_elem index);
int		handle_default_error(void **scene_elem, void *str1, void *str2);
int		pre_elem_check(void **target, char *line, char **parse_line, e_type_elem type);
//common_utils_2.c
int		parse_coordinates(char *xyz_str, float xyz_target[3]);
int		parse_coordinates_vector(char *vector_str, float vector_target[3]);
int		parse_fov(char *fov_str, int *fov_target);
int		validate_fov_str(char *fov_str);
//common_utils_3.c
int		sign_handler(char **target_str);
//error_handlers.c
int		open_error_cases(char *filename, int errno_code);
void	print_element(void *elem, e_type_elem type);
void	print_spheres(t_scene *scene);
//fill_ambiance.c
int		fill_ambiance(t_scene *scene, char *line);
//fill_camera.c
int		fill_camera(t_scene *scene, char *line);
//fill_light.c
int		fill_light(t_scene *scene, char *line);
//fill_sphere.c
int		fill_sphere(t_scene *scene, char *line);
//fill_utils.c
size_t	get_elem_size(e_type_elem type);
char	*get_type_name(e_type_elem type);
//init_objects.c
int		init_objects(t_scene *scene);
//parser.c
int		check_filename(char *filename);
int		file_management(char *filename);
int		parse_file(t_scene	*scene, int fd);
//populate_structs.c
int		check_element_id(char *id, t_scene *scene, char *line);
int		tokenize_line(char *line, t_scene *scene);
//validate_array_utils.c
int		validate_rgb_components(char **components, char *rgb_str);
int		validate_coordinates(char **coordinates, char *xyz_str);

#endif