/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:36:51 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/29 23:11:58 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H
# define GET_OCCURRENCE -1
# define OBJ_INITIAL_CAPACITY 8
# define M_PI 3.14159265358979323846
# define EPS 1e-8
# define KEY_ESC 65307
# define SHADOW_EPS 1e-3f
# define SQUARE_PATTERN_SCALE 5.0f
# define CYLINDER_CHECKER_FREQUENCY 20.0f //minimum for a real square pattern

# include <math.h>
# include <float.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <pthread.h>
# include <stdint.h>
# include <sys/time.h>
# include "libft.h"
# include "mlx.h"
# include "mlx_int.h"

typedef enum e_type
{
	AMBIENT,
	CAMERA,
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER,
	PARABOLOID,
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
	int		checkerboard;//init where?
}	t_sphere;

typedef struct s_plane
{
	char	*id;				//pl
	float	pl_xyz[3];			//p0
	float	pl_vector_xyz[3];	//N
	int		pl_rgb[3];			//color
	int		checkerboard;//init where?
}	t_plane;

typedef struct s_cylinder
{
	char	*id; //cy
	float	cy_xyz[3];
	float	cy_vector_xyz[3];
	float	cy_diameter;
	float	cy_height;
	int		cy_rgb[3];
	int		checkerboard;//init where?
}	t_cylinder;

//PARABOLOID STRUCT FOR BONUS
typedef struct s_paraboloid
{
	float	center[3];
	float	axis[3];
	int		rgb[3];
	float	k;//steepness
	float	height;
	char	*id;
}	t_paraboloid;

typedef struct s_scene
{
	t_ambient		*ambiance;
	t_camera		*camera;
	t_light			*light;
	t_sphere		**sphere;
	t_plane			**plane;
	t_cylinder		**cylinder;
	t_paraboloid	**paraboloid;
	int				sphere_capacity;
	int				plane_capacity;
	int				cylinder_capacity;
	int				paraboloid_capacity;
}	t_scene;

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
	float		hit_point[3];
	float		normal[3];
	int			color[3];
	float		t; //distance from origin to hit
	int			hit; //true/false 1/0
	int			object_idx;//in the array of objects
	int			obj_scene_idx;
	e_type_elem	object_type;
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

typedef struct s_render_ctx
{
	t_hit	*rec;
	int		a_shade[3];
	int		c_obj_term[3];
	int		c_light_term[3];
	float	final_shade[3];
	int		out_shade[3];
	int		shade_to_hex[3];
	int		color;
	int		x;
	int		y;
	int		i;
	float	co[3]; //objecto color normalized
	float	cl[3]; //light color normalized
	float	lamb_weight;
	float	n[3];
	float	side;
}	t_render_ctx;

typedef struct s_sp_ctx
{
	t_sphere	*curr_sp;
	t_hit		*rec;
	float		dir[3];
	int			i;
	int			temp_scene_idx;
}	t_sp_ctx;

typedef struct s_pl_ctx
{
	t_plane	*curr_pl;
	t_hit	*rec;
	float	d[3];
	float	normal[3];
	float	origin[3];
	float	p0[3];
	float	diff[3];
	float	scale_td[3];
	float	hit_p[3];
	double	denom;
	int		i;
	int		s;
	int		temp_scene_idx;
}	t_pl_ctx;

typedef struct s_cy_ctx
{
	t_cylinder	*curr_cy;
	t_hit		*rec;
	float		d[3];
	float		normal[3];
	float		origin[3];
	float		base_center[3];
	float		top_center[3];
	float		ha[3];
	float		v[3];
	float		w[3];
	float		y;
	float		half_height;
	double		radius;
	int			i;
	int			s;
	int			temp_scene_idx;
}	t_cy_ctx;

typedef struct s_cy_cap_ctx
{
	float	center[3];
	float	hit_cap_p[3];
	float	cap_normal[3];
	double	tcap;
	float	radius;
	float	hit_cap_point[3];
	double	denom;
	double	hit_cap;
	float	r;
}	t_cy_cap;

typedef struct s_abc
{
	double	a;
	double	b;
	double	c;
}	t_abc;

typedef struct s_screen_ndc_ctx
{
	float	u;
	float	v;
	int		x_index;
	int		y_index;
	int		index;
}	t_screen_ndc_ctx;

typedef struct s_app
{
	t_scene		*scene;
	t_window	*win;
	t_ray_table	*ray_table;
	int			click_lock;//to prevent spam clicks while rendering
}	t_app;

// y_start and y_end will be our horizontal delimiters

typedef struct s_thread_data
{
	t_app			*app;
	int				y_start;
	int				y_end;
}	t_thread_data;

//clean_utils.c
void	free_split(char **parts);
void	sanitize_gnl(int fd);
void	cleanup_all(t_scene *scene);
//common_utils.c
int		parse_rgb(char *rgb_str, int rgb_target[3], e_type_elem type);
int		parse_light_ratio(char *ratio_str, float *light_ratio,
			e_type_elem type);
int		set_and_get_occ(int value, e_type_elem index);
int		handle_default_error(void **scene_elem, void *str1, void *str2);
int		pre_elem_check(void **target, char *line, char **parse_line,
			e_type_elem type);
//common_utils_2.c
int		parse_coordinates(char *xyz_str, float xyz_target[3]);
int		parse_coordinates_vector(char *vector_str, float vector_target[3]);
int		parse_fov(char *fov_str, float *fov_target);
int		validate_fov_str(char *fov_str);
//common_utils_3.c
int		sign_handler(char **target_str);
int		parse_diameter(char *diameter_str, float *diameter_target,
			e_type_elem type);
int		validate_diameter_str(char *diameter_str);
int		check_dots(char *diameter_str, int i);
long	get_time_ms(void);
//common_utils_4.c
int		parse_steepness(char *steep_str, float *steep_target, e_type_elem type);
void	clean_paraboloid_and_cylinder(t_scene *scene);
//create_vectors.c
int		create_rays(t_camera *camera, t_window *win, t_ray_table *ray_table);
//create_vectors_utils.c
int		cross(float *a_xyz, float *b_xyz, float *target_xyz);
int		init_rays(t_ray_table *ray_table);
int		calc_vectors(t_ray_table *ray_table, t_cam_basis *cam_basis,
			t_window *win);
int		print_array3(float *target_xyz);
//cylinder_caps.c
double	cylinder_bottom_cap(t_cy_ctx *cy_ctx, int flag);
double	cylinder_top_cap(t_cy_ctx *cy_ctx, int flag);
//cylinder_caps_utils.c
void	save_intersection_in_table(t_cy_ctx *cy_ctx, float *hit_p,
			double t_side);
void	compute_cylinder_finite_height(double t_side, t_cy_ctx *cy_ctx);
void	calc_v_w(t_cy_ctx *cy_ctx);
double	solve_cylinder_formula(t_scene	*scene, t_cy_ctx *cy_ctx);
//cylinder_shadow_rays_utils.c
double	cylinder_top_sr(t_cy_ctx *cy_ctx, float t_max);
double	cylinder_bottom_sr(t_cy_ctx *cy_ctx, float t_max);
void	prep_sr_cy_intersect(t_cy_ctx *cy_ctx, float *sr_origin, float *sr_dir);
float	comp_finite_height_for_light(float t_side, t_cy_ctx *cy_ctx,
			float t_max);
float	ray_intersection_cy(float *sr_origin, float *sr_dir,
			t_cylinder *cy, float distance);
//cylinder_intersection_utils.c
void	init_curr_iter_values(t_scene *scene, t_cy_ctx *cy_ctx);
double	solve_t_cylinder(float v[3], float w[3], t_cy_ctx *cy_ctx);
int		render_cylinder(t_ray_table *ray_table, t_scene *scene, t_window *win);
int		ray_cylinder_intersection(t_ray_table *ray_table, t_scene *scene);
//error_handlers.c
int		open_error_cases(char *filename, int errno_code);
void	print_element(void *elem, e_type_elem type);
void	print_spheres(t_scene *scene);
void	print_planes(t_scene *scene);
void	print_cylinders(t_scene *scene);
void	print_paraboloid(t_scene *scene);
//fill_ambiance.c
int		fill_ambiance(t_scene *scene, char *line);
//fill_camera.c
int		fill_camera(t_scene *scene, char *line);
//fill_cylinder.c
int		validate_height_str(char *height_str);
int		parse_height(char *height_str, float *height_target);
int		fill_cylinder(t_scene *scene, char *line);
//fill_light.c
int		fill_light(t_scene *scene, char *line);
//fill_plane.c
int		fill_plane(t_scene *scene, char *line);
//fill_paraboloid.c
int		fill_paraboloid(t_scene *scene, char *line);
//fill_sphere.c
int		fill_sphere(t_scene *scene, char *line);
//fill_utils.c
size_t	get_elem_size(e_type_elem type);
char	*get_type_name(e_type_elem type);
//init_hit_record.c
void	init_hit_record(t_ray_table *ray_table);
//init_objects.c
int		init_objects(t_scene *scene);
//render_loop.c
void	*render_thread(void *data);
void	start_multithread_render(t_app *app);
void	render_objects(t_app *app);
//shading_utils.c
int		apply_shade_to_pixel(t_app *app, t_hit *hit);
//sphere_render.c
int		render_sphere(t_ray_table *ray_table, t_scene *scene, t_window *win);
//math_operations.c
int		sign(double x);
//light_management.c
int		apply_diffuse_specular_and_shadow(t_render_ctx *render, t_scene *scene,
			t_window *win);
//light_management_utils.c
float	prep_shadow_ray(t_scene *scene, t_render_ctx *render, float *dir,
			float *p_offset);
double	ray_intersection_pl_shadow(float *sr_origin, float *sr_dir,
			t_plane *pl, float distance);
float	ray_intersection_sp(float *sr_origin, float *sr_dir, t_sphere *sphere);
//light_phong.c
float	apply_specular_light(t_scene *scene, t_render_ctx *render);
int		combine_lights(t_render_ctx *render, t_scene *scene, float *dir);
void	apply_ambient_light(t_scene *scene, t_hit *curr_rec,
			t_render_ctx *render);
//parser.c
int		check_filename(char *filename);
int		file_management(char *filename);
int		parse_file(t_scene	*scene, int fd);
//plane_intersection_utils.c
double	solve_pl_formula(t_scene *scene, t_pl_ctx *pl_ctx, int flag);
//plane_intersection.c
int		ray_plane_intersect(t_ray_table *ray_table, t_scene *scene);
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
double	solve_discriminant(float e[3], float d[3], t_sp_ctx *sp_ctx, int flag);
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
//vector_operations_2.c
void	copy_vectors(float out[3], float in[3]);
void	copy_int_vectors(int out[3], int in[3]);
void	mult(float target_xyz[3], float a[3], float b[3]);
void	normalize_colors(float rgb[3]);
void	normalize_target_colors(float target_xyz[3], int rgb[3]);
//vector_operations_3.c
int		cross(float *a_xyz, float *b_xyz, float *target_xyz);
double	ray_length(float vector[3]);
void	set_vec_int_values(int vec[3], int va, int vb, int vc);
void	set_vec_float_values(float vec[3], float va, float vb, float vc);
//key_events.c
int		close_window(t_app *app);
int		key_press(int keycode, t_app *app);
int		double_left_click(int keycode, int x, int y, t_app *app);
//apply_checkerboard.c
int		apply_checkerboard(t_hit *hit);
void	apply_checkboard_for_sphere(t_hit *hit, t_sphere *sp, int target[3]);
void	apply_checkerboard_for_plane(t_hit *hit, t_plane *pl, int target[3]);
void	apply_checkerboard_cy(t_hit *hit, t_cylinder *cy, int target[3]);
//apply_checkerboard_helper.c
void	apply_matrix(float result[3], float m[4][4], float p[3]);
void	fill_inv_matrix_helper(float m[4][4], float axis[3], float right[3],
			float forward[3]);
void	fill_inv_matrix(float m[4][4], t_cylinder *cy);
void	reverse_checkboard_pattern(t_render_ctx *render, t_scene *scene);
//click_event_bonus.c
void	handle_click(int x, int y, t_app *app);

//STRUCTS FOR BONUS
typedef struct s_ck_ctx
{
	float	p[3];
	float	radius;
	float	theta;
	float	phi;
	float	u;
	float	v;
	int		u_i;
	int		v_i;
}	t_ckboard_sp_ctx;

typedef struct s_ck_ctk
{
	float	mx[4][4];
	float	tetha;
	float	rawU;
	float	u;
	float	p[3];
	float	v;
	int		iu;
	int		iv;
}	t_ckboard_cy_ctx;

#endif