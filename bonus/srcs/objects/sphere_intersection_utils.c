/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_intersection_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:19:52 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 21:24:59 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	solve_t(t_abc *abc)
{
	double	delta;
	double	t;
	double	q;
	double	t0;
	double	t1;

	delta = abc->b * abc->b - (4.0 * (abc->a * abc->c));
	if (delta < 0.0)
		return (-1.0);
	q = -0.5 * (abc->b + (double)sign(abc->b) * sqrt(delta));
	t0 = q / abc->a;
	t1 = abc->c / q;
	if (t0 > t1)
	{
		t = t0;
		t0 = t1;
		t1 = t;
	}
	if (t1 < EPS)
		return (-1.0);
	if (t0 > EPS)
		t = t0;
	else
		t = t1;
	return (t);
}

/// @brief 
/// @param e 
/// @param d 
/// @param sphere 
/// @param abc A = d * d; B = scale_tmp * sub_tmp; C = sub_tmp² - R²
/// @return 
int	solve_abc(double e[3], double d[3], t_sphere *sphere, t_abc *abc)
{
	double	scale_tmp[3];
	double	sub_tmp[3];
	double	radius;

	abc->a = (double)dot(d, d);
	scale(scale_tmp, d, 2.0f);
	sub(sub_tmp, e, sphere->sp_center_xyz);
	abc->b = dot(scale_tmp, sub_tmp);
	radius = (double)sphere->sp_diameter * 0.5;
	abc->c = dot(sub_tmp, sub_tmp) - (radius * radius);
	return (1);
}

int	update_hit_record(double t, double e[3], double d[3], t_sp_ctx *sp_ctx)
{
	double	hit_p[3];
	double	normal[3];

	if (!sp_ctx->rec)
		return (0);
	if (sp_ctx->rec->hit && (double)sp_ctx->rec->t <= t + EPS)
		return (0);
	scale(hit_p, d, t);
	add(hit_p, e, hit_p);
	sub(normal, hit_p, sp_ctx->curr_sp->sp_center_xyz);
	normalize(normal, normal);
	sp_ctx->rec->t = (double)t;
	sp_ctx->rec->hit = 1;
	sp_ctx->rec->object_idx = sp_ctx->i;
	copy_vectors(sp_ctx->rec->hit_point, hit_p);
	copy_vectors(sp_ctx->rec->normal, normal);
	copy_vectors(sp_ctx->rec->shading_normal, normal);
	copy_int_vectors(sp_ctx->rec->color, sp_ctx->curr_sp->sp_rgb);
	sp_ctx->rec->object_type = SPHERE;
	return (1);
}

/// @brief Function that solves the discriminant formula: finds the closest
/// @brief intersection value 't', the function should extract information
/// @brief about the surface that was hit. Values defined in the t_hit struct
/// @brief at minirt.h header file, the most important ones are hitpoint and
/// @brief 't' (distance), so in the final renderization steps we can use to
/// @brief apply hard shadows and create a ray from that position towards light
/// @param e Ray origin point (camera-eye coordinates)
/// @param d Ray direction normalized, vectors ZYX in ray_table
/// @param sp_ctx Context variables store in just one place for simplicity
/// @return 1 on success, 0 on failure, FIND 't' (closest hit point)
double	solve_discriminant(double e[3], double d[3], t_sp_ctx *sp_ctx, int flag)
{
	t_abc	abc;
	double	t;

	solve_abc(e, d, sp_ctx->curr_sp, &abc);
	t = solve_t(&abc);
	if (t < 0.0f)
		return (0.0);
	if (flag == 1)
	{
		sp_ctx->rec->obj_scene_idx = sp_ctx->temp_scene_idx;
		update_hit_record(t, e, d, sp_ctx);
	}
	return (t);
}

/// @brief 1. Loops over all rays in ray_table.
/// @brief 2. For each ray, loops over all spheres in scene..
/// @brief 3. Reset hit record for current/frame, so data don't get staled
/// @brief 4. Computes intersection. solve_discriminant()
/// @brief 6. Apply to every sphere in the file.
/// @brief 5. Updates the corresponding t_hit record with the closest hit
/// @param ray_table 
/// @param scene 
/// @return 
int	ray_sphere_intersect(t_ray_table *ray_table, t_scene *scene)
{
	t_sp_ctx	sp_ctx;
	int			s;

	sp_ctx.i = 0;
	while (sp_ctx.i < ray_table->total_rays)
	{
		sp_ctx.dir[0] = ray_table->vectors_x[sp_ctx.i];
		sp_ctx.dir[1] = ray_table->vectors_y[sp_ctx.i];
		sp_ctx.dir[2] = ray_table->vectors_z[sp_ctx.i];
		sp_ctx.rec = &ray_table->hit_record[sp_ctx.i];
		s = 0;
		while (s < scene->sphere_capacity && scene->sphere[s])
		{
			sp_ctx.curr_sp = scene->sphere[s];
			sp_ctx.temp_scene_idx = s;
			solve_discriminant(scene->camera->coordinates_xyz, sp_ctx.dir,
				&sp_ctx, 1);
			s++;
		}
		sp_ctx.i++;
	}
	return (1);
}
