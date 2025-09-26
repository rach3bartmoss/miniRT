/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:36:51 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/26 09:28:23 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H
# define GET_OCCURRENCE -1
# define OBJ_INITIAL_CAPACITY 8
# define M_PI 3.14159265358979323846
# define EPS 1e-8
# define KEY_ESC 65307

# include <math.h>
# include <float.h>
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
	float		coordinates_xyz[3];			//origin point of the camera
	float		vector_xyz[3];	//Orientation of each axis XYZ [0.0~1.0]
	float		fov;			//horizontal field view [0-180]
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
	char	*id;				//pl
	float	pl_xyz[3];			//p0
	float	pl_vector_xyz[3];	//N
	int		pl_rgb[3];			//color
}	t_plane;

typedef struct s_cylinder
{
	char	*id; //cy
	float	cy_xyz[3];
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

typedef struct s_bbox
{
	float	min[3];
	float	max[3];
}	t_bbox;

typedef struct s_bvh_node
{
	t_bbox				bounds;
	struct s_bvh_node	*left;
	struct s_bvh_node	*right;
	int					start;
	int					count;
}	t_bvh_node;

typedef struct s_window
{
	void	*mlx;
	void	*win;
	float	aspect_ratio;
	int		width;
	int		height;
	void	*img;
	char	*addr;
	int		bpp;
	int		line_length;
	int		endian;
}	t_window;

/// @brief Holds the camera coordinate system values.
typedef struct s_camera_basis
{
	float	forward[3];
	float	right[3];
	float	up[3];
	float	half_width;// = tanf(fov_radian/2)
	float	half_height;// = half_width / aspect_ratio
}	t_cam_basis;

typedef struct s_hit
{
	float	t; //distance from origin to hit
	int		hit; //true/false 1/0
	int		object_idx;//in the array of objects
	float	hit_point[3];
	float	normal[3];
	int		color[3];
}	t_hit;

typedef struct s_ray_table
{
	t_hit	*hit_record;
	float	*vectors_x;
	float	*vectors_y;
	float	*vectors_z;
	float	half_width;
	float	half_height;
	int		total_rays;
}	t_ray_table;

typedef struct s_sp_ctx
{
	t_sphere	*curr_sp;
	t_hit		*rec;
	float		dir[3];
	int			i;
}	t_sp_ctx;

typedef struct s_pl_ctx
{
	t_plane	*curr_pl;
	t_hit	*rec;
	float	d[3];
	float	normal[3];
	float	origin[3];
	float	p0[3];
	double	denom;
	int		i;
	int		s;
	
}	t_pl_ctx;

typedef struct s_abc
{
	double	A;
	double	B;
	double	C;
}	t_abc;

typedef struct s_app
{
	t_scene		*scene;
	t_window	*win;
	t_ray_table	*ray_table;
}	t_app;

//clean_utils.c
void	free_split(char **parts);
void	sanitize_gnl(int fd);
void	cleanup_all(t_scene *scene);
//common_utils.c
int		parse_rgb(char *rgb_str, int rgb_target[3], e_type_elem type);
int		parse_light_ratio(char *ratio_str, float *light_ratio, e_type_elem type);
int		set_and_get_occ(int value, e_type_elem index);
int		handle_default_error(void **scene_elem, void *str1, void *str2);
int		pre_elem_check(void **target, char *line, char **parse_line, e_type_elem type);
//common_utils_2.c
int		parse_coordinates(char *xyz_str, float xyz_target[3]);
int		parse_coordinates_vector(char *vector_str, float vector_target[3]);
int		parse_fov(char *fov_str, float *fov_target);
int		validate_fov_str(char *fov_str);
//common_utils_3.c
int		sign_handler(char **target_str);
//create_vectors.c
int		create_rays(t_camera *camera, t_window *win, t_ray_table *ray_table);
//create_vectors_utils.c
int		cross(float *a_xyz, float *b_xyz, float *target_xyz);
int		init_rays(t_ray_table *ray_table);
int		calc_vectors(t_ray_table *ray_table, t_cam_basis *cam_basis, t_window *win);
int		print_array3(float *target_xyz);
//error_handlers.c
int		open_error_cases(char *filename, int errno_code);
void	print_element(void *elem, e_type_elem type);
void	print_spheres(t_scene *scene);
void	print_planes(t_scene *scene);
void	print_cylinders(t_scene *scene);
//fill_ambiance.c
int		fill_ambiance(t_scene *scene, char *line);
//fill_camera.c
int		fill_camera(t_scene *scene, char *line);
//fill_cylinder.c
int		fill_cylinder(t_scene *scene, char *line);
//fill_light.c
int		fill_light(t_scene *scene, char *line);
//fill_plane.c
int		fill_plane(t_scene *scene, char *line);
//fill_sphere.c
int		check_dots(char *diameter_str, int i);
int		validate_diameter_str(char *diameter_str);
int		parse_diameter(char *diameter_str, float *diameter_target, e_type_elem type);
int		fill_sphere(t_scene *scene, char *line);
//fill_utils.c
size_t	get_elem_size(e_type_elem type);
char	*get_type_name(e_type_elem type);
//init_hit_record.c
void	init_hit_record(t_ray_table *ray_table);
//init_objects.c
int		init_objects(t_scene *scene);
//intersections.c
int		render_sphere(t_ray_table *ray_table, t_scene *scene, t_window *win);
//math_operations.c
int		sign(double x);
//parser.c
int		check_filename(char *filename);
int		file_management(char *filename);
int		parse_file(t_scene	*scene, int fd);
//plane_intersection_utils.c
int		render_plane(t_ray_table *ray_table, t_scene *scene, t_window *win);
//populate_structs.c
int		check_element_id(char *id, t_scene *scene, char *line);
int		tokenize_line(char *line, t_scene *scene);
//put_pixel_utils.c
void	mrt_put_pixel(t_window *win, int x, int y, int color);
int		rgb3_to_hex(int rgb[3]);
//sphere_intersection_utils.c
double	solve_t(t_abc *abc);
int		solve_abc(float e[3], float d[3], t_sphere *sphere, t_abc *abc);
int		update_hit_record(double t, float e[3], float d[3], t_sp_ctx *sp_ctx);
int		solve_discriminant(float e[3], float d[3], t_sp_ctx *sp_ctx);
int		ray_sphere_intersect(t_ray_table *ray_table, t_scene *scene);
//validate_array_utils.c
int		validate_rgb_components(char **components, char *rgb_str);
int		validate_coordinates(char **coordinates, char *xyz_str);
//vector_operations.c
double	dot(float a[3], float b[3]);
void	sub(float *target, float *a, float *b);
void	add(float *target, float *a, float *b);
void	scale(float *target, float *a, float scale_factor);
int		normalize(float *vector_xyz, float *target_xyz);
//vector_operations.c
void	copy_vectors(float out[3], float in[3]);
void	copy_int_vectors(int out[3], int in[3]);
//key_events.c
int		close_window(t_app *app);
int		key_press(int keycode, t_app *app);
#endif