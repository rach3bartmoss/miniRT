/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersection_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 10:24:03 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/27 00:05:03 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief Run basic calculations that are used in each iteration such as:
/// @brief 1) Normalize cylinder axis orientation vector (normal).
/// @brief 2) Get the ray origin coordinates.
/// @brief 3) Get the half_height and radius of the cylinder.
/// @brief 4) Calculate the 'hA' to apply in base and top calculations.
/// @brief 5) Calculate base and top positions.
/// @param scene Scene variables, access to each object in the .rt file
/// @param cy_ctx Context variables
void	init_curr_iter_values(t_scene *scene, t_cy_ctx *cy_ctx)
{
	copy_vectors(cy_ctx->normal, cy_ctx->curr_cy->cy_vector_xyz);
	normalize(cy_ctx->normal, cy_ctx->normal);
	copy_vectors(cy_ctx->origin, scene->camera->coordinates_xyz);
	cy_ctx->half_height = cy_ctx->curr_cy->cy_height / 2.0f;
	cy_ctx->radius = cy_ctx->curr_cy->cy_diameter / 2.0f;
	scale(cy_ctx->ha, cy_ctx->normal, cy_ctx->half_height);
	sub(cy_ctx->base_center, cy_ctx->curr_cy->cy_xyz, cy_ctx->ha);
	add(cy_ctx->top_center, cy_ctx->curr_cy->cy_xyz, cy_ctx->ha);
}

/// @brief Solve t using the quadratic formula and pick the smallest
/// @brief positive root, uses the v and w to check if the intersection
/// @brief satisfy:
/// @brief 1) |vt + w|² = r² -> at² + bt + c (quadratic equation)
/// @brief 2) a = v * v
/// @brief 3) b = 2(v * w)
/// @brief 4) c = w * w - r²
/// @param v value after the axis removed from ray direction
/// @param w value after the axis removed from origin-to-cylinder
/// @param cy_ctx 
/// @return return the smallest positive root, t1 or t2
double	solve_t_cylinder(float v[3], float w[3], t_cy_ctx *cy_ctx)
{
	t_abc	abc;
	double	t1;
	double	t2;
	double	delta;
	double	root;

	abc.a = dot(v, v);
	abc.b = dot(v, w) * 2.0;
	abc.c = dot(w, w) - (cy_ctx->radius * cy_ctx->radius);
	delta = (abc.b * abc.b) - (4.0 * (abc.a * abc.c));
	if (delta < 0.0 || fabs(abc.a) < 1e-12)
		return (-1.0);
	root = sqrt(delta);
	t1 = (-abc.b - root) / (2.0 * abc.a);
	t2 = (-abc.b + root) / (2.0 * abc.a);
	if (t1 > 0.0 && t2 > 0.0)
		return (fmin(t1, t2));
	if (t1 > 0.0)
		return (t1);
	if (t2 > 0.0)
		return (t2);
	return (-1.0);
}

int	ray_cylinder_intersection(t_ray_table *ray_table, t_scene *scene)
{
	t_cy_ctx	cy_ctx;

	cy_ctx.i = 0;
	while (cy_ctx.i < ray_table->total_rays)
	{
		cy_ctx.d[0] = ray_table->vectors_x[cy_ctx.i];
		cy_ctx.d[1] = ray_table->vectors_y[cy_ctx.i];
		cy_ctx.d[2] = ray_table->vectors_z[cy_ctx.i];
		cy_ctx.rec = &ray_table->hit_record[cy_ctx.i];
		cy_ctx.s = 0;
		while (cy_ctx.s < scene->cylinder_capacity && scene->cylinder[cy_ctx.s])
		{
			cy_ctx.curr_cy = scene->cylinder[cy_ctx.s];
			solve_cylinder_formula(scene, &cy_ctx);
			cy_ctx.s++;
		}
		cy_ctx.i++;
	}
	return (1);
}

//mlx_pixel_put(win->mlx, win->win, x, y, 0x000000);
void	render_cylinder_loop(t_ray_table *ray_table, t_window *win)
{
	t_hit	*rec;
	int		color;
	int		x;
	int		y;
	int		i;

	y = 0;
	while (y < win->height)
	{
		x = 0;
		while (x < win->width)
		{
			i = y * win->width + x;
			rec = &ray_table->hit_record[i];
			if (rec->hit)
			{
				color = rgb3_to_hex(rec->color);
				mrt_put_pixel(win, x, y, color);
			}
			else
				mrt_put_pixel(win, x, y, 0x000000);
			x++;
		}
		y++;
	}
}

int	render_cylinder(t_ray_table *ray_table, t_scene *scene, t_window *win)
{
	ray_cylinder_intersection(ray_table, scene);
	render_cylinder_loop(ray_table, win);
	mlx_put_image_to_window(win->mlx, win->win, win->img, 0, 0);
	return (1);
}
