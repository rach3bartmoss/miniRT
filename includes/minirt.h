/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:36:51 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/24 00:03:12 by joao-vri         ###   ########.fr       */
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
# define TEXTURE_CAPACITY 12
# define PATH_MAX_SIZE 4096
# define PRESET_N 12
# define TEXTURE_SCALE 0.1f
# define BUMP_STRENGTH 0.8F
# define TEXTURE_N 24 //(PRESET * 2)

# include <math.h>
# include <float.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <pthread.h>
# include <stdint.h>
# include <sys/time.h>
# include <limits.h>
# include <termios.h>
# include <signal.h>
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
}	t_type_elem;

typedef enum e_tex_type
{
	BASE,
	BUMP,
	DEFAULT,
}	t_type_tex;

typedef struct s_preset
{
	char	*name;
	char	*base;
	char	*bump;
}	t_preset;

typedef struct s_texture
{
	int			width;
	int			height;
	int			bpp;//bits per pixel
	int			line_len;
	int			endian;
	char		*buffer;
	char		*path;
	void		*img;
	t_type_tex	type;
	t_preset	*preset;//pointer to preset values in the preset_list
}	t_texture;

typedef struct s_texture_pair
{
	t_texture	*base;
	t_texture	*bump;
}	t_tex_pair;

typedef struct s_ambient
{
	char		id; //A
	double		light_ratio;	//range between 0.0 - 1.0
	int			a_rgb[3];			//[0-255]
}	t_ambient;

typedef struct s_camera
{
	char		id; //C
	double		coordinates_xyz[3];			//origin point of the camera
	double		vector_xyz[3];	//Orientation of each axis XYZ [0.0~1.0]
	double		fov;			//horizontal field view [0-180]
}	t_camera;

typedef struct s_light
{
	char		id; //L
	double		light_xyz[3];
	double		bright_ratio;		//[0.0~1.0]
	int			light_rgb[3];			//[0-255]
}	t_light;

typedef struct s_sphere
{
	char		*id; //sp
	double		sp_center_xyz[3];
	double		sp_diameter;
	int			sp_rgb[3];
	int			checkerboard;
	t_texture	*base;
	t_texture	*bump;
}	t_sphere;

typedef struct s_plane
{
	char		*id;				//pl
	double		pl_xyz[3];			//p0
	double		pl_vector_xyz[3];	//N
	int			pl_rgb[3];			//color
	int			checkerboard;
	t_texture	*base;
	t_texture	*bump;
}	t_plane;

typedef struct s_cylinder
{
	char		*id; //cy
	double		cy_xyz[3];
	double		cy_vector_xyz[3];
	double		cy_diameter;
	double		cy_height;
	int			cy_rgb[3];
	int			checkerboard;
	t_texture	*base;
	t_texture	*bump;
}	t_cylinder;

//PARABOLOID STRUCT FOR BONUS
typedef struct s_paraboloid
{
	double	center[3];
	double	axis[3];
	int		rgb[3];
	double	k;//steepness
	double	height;
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
	double	aspect_ratio;
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
	double	forward[3];
	double	right[3];
	double	up[3];
	double	half_width;// = tanf(fov_radian/2)
	double	half_height;// = half_width / aspect_ratio
}	t_cam_basis;

typedef struct s_hit
{
	double		hit_point[3];
	double		normal[3];
	double		shading_normal[3];
	int			color[3];
	double		t; //distance from origin to hit
	int			hit; //true/false 1/0
	int			object_idx;//in the array of objects
	int			obj_scene_idx;
	t_type_elem	object_type;
}	t_hit;

typedef struct s_ray_table
{
	t_hit	*hit_record;
	double	*vectors_x;
	double	*vectors_y;
	double	*vectors_z;
	double	half_width;
	double	half_height;
	int		total_rays;
}	t_ray_table;

typedef struct s_render_ctx
{
	t_hit	*rec;
	int		a_shade[3];
	int		c_obj_term[3];
	int		c_light_term[3];
	double	final_shade[3];
	int		out_shade[3];
	int		shade_to_hex[3];
	int		color;
	int		x;
	int		y;
	int		i;
	double	co[3]; //objecto color normalized
	double	cl[3]; //light color normalized
	double	lamb_weight;
	double	n[3];
	double	side;
}	t_render_ctx;

typedef struct s_sp_ctx
{
	t_sphere	*curr_sp;
	t_hit		*rec;
	double		dir[3];
	int			i;
	int			temp_scene_idx;
}	t_sp_ctx;

typedef struct s_pl_ctx
{
	t_plane	*curr_pl;
	t_hit	*rec;
	double	d[3];
	double	normal[3];
	double	origin[3];
	double	p0[3];
	double	diff[3];
	double	scale_td[3];
	double	hit_p[3];
	double	denom;
	int		i;
	int		s;
	int		temp_scene_idx;
}	t_pl_ctx;

typedef struct s_cy_ctx
{
	t_cylinder	*curr_cy;
	t_hit		*rec;
	double		d[3];
	double		normal[3];
	double		origin[3];
	double		base_center[3];
	double		top_center[3];
	double		ha[3];
	double		v[3];
	double		w[3];
	double		y;
	double		half_height;
	double		radius;
	int			i;
	int			s;
	int			temp_scene_idx;
}	t_cy_ctx;

typedef struct s_cy_cap_ctx
{
	double	center[3];
	double	hit_cap_p[3];
	double	cap_normal[3];
	double	tcap;
	double	radius;
	double	hit_cap_point[3];
	double	denom;
	double	hit_cap;
	double	r;
}	t_cy_cap;

typedef struct s_pa_ctx
{
	t_paraboloid	*pa;
	t_texture		*base;
	t_texture		*bump;
	int				i; // index
	double			d[3];
	double			o[3];
	double			n[3];
	double			w[3];
	double			p[3];
	double			n_surface[3];
	double			dot_dn; // height d
	double			dot_wn; // height w
	double			h; // height
	double			a; // radial speed
	double			b; // interaction
	double			c; // relative position
	double			discr; // discriminant
	double			t; // distance
}	t_pa_ctx;

typedef struct s_abc
{
	double	a;
	double	b;
	double	c;
}	t_abc;

typedef struct s_screen_ndc_ctx
{
	double	u;
	double	v;
	int		x_index;
	int		y_index;
	int		index;
}	t_screen_ndc_ctx;

typedef struct s_r_click_ctx
{
	int			index;
	t_hit		*hit;
	t_preset	*target_preset;
	t_tex_pair	*target_pair;
}	t_r_click_ctx;

typedef struct s_app
{
	struct sigaction	sa;
	t_scene				*scene;
	t_window			*win;
	t_ray_table			*ray_table;
	t_texture			**textures;
	t_preset			**preset_list;
	int					n_textures;
	int					tex_capacity;
	int					click_lock;//to prevent spam clicks while rendering
}	t_app;

// y_start and y_end will be our horizontal delimiters

typedef struct s_thread_data
{
	t_app			*app;
	int				y_start;
	int				y_end;
}	t_thread_data;

//clean_utils.c
void		free_split(char **parts);
void		sanitize_gnl(int fd);
void		cleanup_all(t_scene *scene);
//common_utils.c
int			parse_rgb(char *rgb_str, int rgb_target[3], t_type_elem type);
int			parse_light_ratio(char *ratio_str, double *light_ratio,
				t_type_elem type);
int			set_and_get_occ(int value, t_type_elem index);
int			handle_default_error(void **scene_elem, void *str1, void *str2);
int			pre_elem_check(void **target, char *line, char **parse_line,
				t_type_elem type);
//common_utils_2.c
int			parse_coordinates(char *xyz_str, double xyz_target[3]);
int			parse_coordinates_vector(char *vector_str, double vector_target[3]);
int			parse_fov(char *fov_str, double *fov_target);
int			validate_fov_str(char *fov_str);
//common_utils_3.c
int			sign_handler(char **target_str);
int			parse_diameter(char *diameter_str, double *diameter_target,
				t_type_elem type);
int			validate_diameter_str(char *diameter_str);
int			check_dots(char *diameter_str, int i);
long		get_time_ms(void);
//common_utils_4.c
int			parse_steepness(char *steep_str, double *steep_target,
				t_type_elem type);
void		clean_paraboloid_and_cylinder(t_scene *scene);
//create_vectors.c
int			create_rays(t_camera *camera, t_window *win,
				t_ray_table *ray_table);
//create_vectors_utils.c
int			cross(double *a_xyz, double *b_xyz, double *target_xyz);
int			init_rays(t_ray_table *ray_table);
int			calc_vectors(t_ray_table *ray_table, t_cam_basis *cam_basis,
				t_window *win);
int			print_array3(double *target_xyz);
//cylinder_caps.c
double		cylinder_bottom_cap(t_cy_ctx *cy_ctx, int flag);
double		cylinder_top_cap(t_cy_ctx *cy_ctx, int flag);
//cylinder_caps_utils.c
void		save_intersection_in_table(t_cy_ctx *cy_ctx, double *hit_p,
				double t_side);
void		compute_cylinder_finite_height(double t_side, t_cy_ctx *cy_ctx);
void		calc_v_w(t_cy_ctx *cy_ctx);
double		solve_cylinder_formula(t_scene	*scene, t_cy_ctx *cy_ctx);
//cylinder_shadow_rays_utils.c
double		cylinder_top_sr(t_cy_ctx *cy_ctx, double t_max);
double		cylinder_bottom_sr(t_cy_ctx *cy_ctx, double t_max);
void		prep_sr_cy_intersect(t_cy_ctx *cy_ctx, double *sr_origin,
				double *sr_dir);
double		comp_finite_height_for_light(double t_side, t_cy_ctx *cy_ctx,
				double t_max);
double		ray_intersection_cy(double *sr_origin, double *sr_dir,
				t_cylinder *cy, double distance);
//cylinder_intersection_utils.c
void		init_curr_iter_values(t_scene *scene, t_cy_ctx *cy_ctx);
double		solve_t_cylinder(double v[3], double w[3], t_cy_ctx *cy_ctx);
int			render_cylinder(t_ray_table *ray_table, t_scene *scene,
				t_window *win);
int			ray_cylinder_intersection(t_ray_table *ray_table, t_scene *scene);
//error_handlers.c
int			open_error_cases(char *filename, int errno_code);
void		print_element(void *elem, t_type_elem type);
void		print_spheres(t_scene *scene);
void		print_planes(t_scene *scene);
void		print_cylinders(t_scene *scene);
void		print_paraboloid(t_scene *scene);
//fill_ambiance.c
int			fill_ambiance(t_scene *scene, char *line);
//fill_camera.c
int			fill_camera(t_scene *scene, char *line);
//fill_cylinder.c
int			validate_height_str(char *height_str);
int			parse_height(char *height_str, double *height_target);
int			fill_cylinder(t_scene *scene, char *line);
//fill_light.c
int			fill_light(t_scene *scene, char *line);
//fill_plane.c
int			fill_plane(t_scene *scene, char *line);
//fill_paraboloid.c
int			fill_paraboloid(t_scene *scene, char *line);
//fill_sphere.c
int			fill_sphere(t_scene *scene, char *line);
//fill_utils.c
size_t		get_elem_size(t_type_elem type);
char		*get_type_name(t_type_elem type);
//init_hit_record.c
void		init_hit_record(t_ray_table *ray_table);
//init_objects.c
int			init_objects(t_scene *scene);
//render_loop.c
void		apply_object_base_color(t_render_ctx *render, t_scene *scene);
void		render_objects(t_app *app);
//render_thread.c
void		*render_thread(void *data);
void		start_multithread_render(t_app *app, int n_threads);
//shading_utils.c
int			apply_shade_to_pixel(t_app *app, t_hit *hit);
//sphere_render.c
int			render_sphere(t_ray_table *ray_table, t_scene *scene,
				t_window *win);
//math_operations.c
int			sign(double x);
//light_management.c
int			apply_diffuse_specular_and_shadow(t_render_ctx *render,
				t_scene *scene, t_window *win);
//light_management_utils.c
double		prep_shadow_ray(t_scene *scene, t_render_ctx *render, double *dir,
				double *p_offset);
double		ray_intersection_pl_shadow(double *sr_origin, double *sr_dir,
				t_plane *pl, double distance);
double		ray_intersection_sp(double *sr_origin, double *sr_dir,
				t_sphere *sphere);
//light_phong.c
double		apply_specular_light(t_scene *scene, t_render_ctx *render);
int			combine_lights(t_render_ctx *render, t_scene *scene, double *dir);
void		apply_ambient_light(t_scene *scene, t_hit *curr_rec,
				t_render_ctx *render);
//parser.c
int			check_filename(char *filename);
int			file_management(char *filename);
int			parse_file(t_scene	*scene, int fd);
//plane_intersection_utils.c
double		solve_pl_formula(t_scene *scene, t_pl_ctx *pl_ctx, int flag);
//plane_intersection.c
int			ray_plane_intersect(t_ray_table *ray_table, t_scene *scene);
int			render_plane(t_ray_table *ray_table, t_scene *scene, t_window *win);
//populate_structs.c
int			check_element_id(char *id, t_scene *scene, char *line);
int			tokenize_line(char *line, t_scene *scene);
//put_pixel_utils.c
void		mrt_put_pixel(t_window *win, int x, int y, int color);
int			rgb3_to_hex(int rgb[3]);
//sphere_intersection_utils.c
double		solve_t(t_abc *abc);
int			solve_abc(double e[3], double d[3], t_sphere *sphere, t_abc *abc);
int			update_hit_record(double t, double e[3], double d[3],
				t_sp_ctx *sp_ctx);
double		solve_discriminant(double e[3], double d[3],
				t_sp_ctx *sp_ctx, int flag);
int			ray_sphere_intersect(t_ray_table *ray_table, t_scene *scene);
//validate_array_utils.c
int			validate_rgb_components(char **components, char *rgb_str);
int			validate_coordinates(char **coordinates, char *xyz_str);
//vector_operations.c
double		dot(double a[3], double b[3]);
void		sub(double *target, double *a, double *b);
void		add(double *target, double *a, double *b);
void		scale(double *target, double *a, double scale_factor);
int			normalize(double *vector_xyz, double *target_xyz);
//vector_operations_2.c
void		copy_vectors(double out[3], double in[3]);
void		copy_int_vectors(int out[3], int in[3]);
void		mult(double target_xyz[3], double a[3], double b[3]);
void		normalize_colors(double rgb[3]);
void		normalize_target_colors(double target_xyz[3], int rgb[3]);
//vector_operations_3.c
int			cross(double *a_xyz, double *b_xyz, double *target_xyz);
double		ray_length(double vector[3]);
void		set_vec_int_values(int vec[3], int va, int vb, int vc);
void		set_vec_double_values(double vec[3], double va,
				double vb, double vc);
//key_events.c
int			close_window(t_app *app);
int			key_press(int keycode, t_app *app);
int			init_right_click_vars(int x, int y, t_app *app, t_r_click_ctx *ctx);
int			mouse_click_handler(int keycode, int x, int y, t_app *app);
//apply_checkerboard.c
int			apply_checkerboard(t_hit *hit);
void		apply_checkboard_for_sphere(t_hit *hit, t_sphere *sp,
				int target[3]);
void		apply_checkerboard_for_plane(t_hit *hit, t_plane *pl,
				int target[3]);
void		apply_checkerboard_cy(t_hit *hit, t_cylinder *cy, int target[3]);
//apply_checkerboard_helper.c
void		apply_matrix(double result[3], double m[4][4], double p[3]);
void		fill_inv_matrix_helper(double m[4][4], double axis[3],
				double right[3], double forward[3]);
void		fill_inv_matrix(double m[4][4], t_cylinder *cy);
void		reverse_checkboard_pattern(t_render_ctx *render, t_scene *scene);
//click_event_bonus.c
void		handle_click(int x, int y, t_app *app);
void		rerender(t_app *app);
//input_textures.c
void		handle_right_click(int x, int y, t_app *app);
//textures_assign.c
t_preset	*input_match_preset_name(t_preset **list, char *line);
void		assign_texture_to_hit_obj(t_app *app, t_hit *hit,
				t_tex_pair *pair);
t_tex_pair	*link_texture_preset(t_preset *preset, t_app *app);
t_preset	*choose_preset(t_preset **list);
//load_textures.c
t_texture	*load_xpm_file(t_app *app, char *filename);
t_texture	*load_texture(t_app *app, char *filename);
int			init_textures(t_app *app);
//textures_utils.c
void		clean_textures_list(t_app *app, t_texture **arr);
void		clean_preset_list(t_preset **arr);
void		free_preset(t_preset *preset);
int			check_preset_values(t_preset *preset);
int			sanitize_preset_line(t_preset **list, char *line);
//signal_handler.c
void		ignore_signal(int sig);
//apply_texture_color.c
double		ft_clamp(double value, double min, double max);
void		get_texture_color(t_texture *tex, int x, int y, int color[3]);
void		apply_textures_for_hit(t_hit *rec, t_scene *scene);
//apply_bump_mapping.c
void		apply_sphere_bump(t_hit *hit, t_texture *bump);
//plane_texture_color.c
void		apply_plane_texture(t_hit *hit, t_texture *tex, t_plane *pl);
//paraboloid_init.c
void		init_paraboloid_ctx(t_paraboloid *curr_pa, t_pa_ctx *curr_ctx,
				t_scene *scene, t_ray_table *ray_table);
void		init_shadow_paraboloid(t_paraboloid *curr_pa, t_pa_ctx *curr_ctx,
				double *new_origin, double *dir);
//paraboloid_calc.c
int			paraboloid_discriminant(t_pa_ctx *curr_ctx);
double		paraboloid_quadratic(t_pa_ctx *curr_ctx);
double		paraboloid_height_test(t_pa_ctx *curr_ctx, double tx);
void		paraboloid_normal(t_pa_ctx *curr_ctx, double t);
//paraboloid_intersection.c
void		ray_paraboloid_intersection(t_ray_table *ray_table, t_scene *scene);
double		ray_intersection_pa_shadow(double *new_origin, double *new_dir,
				t_paraboloid *pa);

//STRUCTS FOR BONUS
typedef struct s_ck_ctx
{
	double	p[3];
	double	radius;
	double	theta;
	double	phi;
	double	u;
	double	v;
	int		u_i;
	int		v_i;
}	t_ckboard_sp_ctx;

typedef struct s_ck_ctk
{
	double	mx[4][4];
	double	tetha;
	double	raw_u;
	double	u;
	double	p[3];
	double	v;
	int		iu;
	int		iv;
}	t_ckboard_cy_ctx;

typedef struct s_light_model_ctx
{
	double	obj_color[3];
	double	light_color[3];
	double	diffuse_part[3];
	double	specular_part[3];
	double	diffuse_factor;
	double	spec_factor;
	int		j;
}	t_light_model_ctx;

typedef struct s_pl_tex_ctx
{
	double	uv[2];
	double	tan[3];
	double	bi_tan[3];
	double	vec[3];
	int		xy[2];
}	t_pl_tex_ctx;

typedef struct s_sp_bump_ctx
{
	double	n[3];
	double	uv[2];
	double	du;
	double	dv;
	double	d_u;
	double	d_v;
	double	h;
	double	hu;
	double	hv;
	double	tangent[3];
	double	bitangent[3];
	double	new_normal[3];
	double	force;
}	t_sp_bump_ctx;

typedef struct s_bump_height_ctx
{
	int				x;
	int				y;
	unsigned int	pixel;
	int				offset;
	double			r;
	double			g;
	double			b;
}	t_bump_height_ctx;

#endif