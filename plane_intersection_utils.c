/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_intersection_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:47:34 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/01 20:53:09 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	plane_occludes(float origin[3], float dir[3], float max_t, t_pl_ctx *P)
{
	float	diff[3];
	float	denom;
	float	t;

	denom = dot(dir, P->normal);
	if (fabs(denom) < 1e-6f)
		return (-1);
	sub(diff, P->p0, origin);
	t = dot(diff, P->normal) / denom;
	return (t > SHADOW_EPS && t < max_t);
}


void	create_iter_pl_ctx(t_scene *scene, t_pl_ctx *pl_ctx)
{
	copy_vectors(pl_ctx->normal, pl_ctx->curr_pl->pl_vector_xyz);
	copy_vectors(pl_ctx->origin, scene->camera->coordinates_xyz);
	copy_vectors(pl_ctx->p0, pl_ctx->curr_pl->pl_xyz);
	pl_ctx->denom = dot(pl_ctx->d, pl_ctx->normal);
}

//ray:		R(t) = O + tD
//plane:	(X - P0) * n = 0
/*	together:
			(O + td - P0) * n = 0
	t	=	(Po - O) * n / D * n

	if (D * N) = 0 ray is parallel to the plane
*/
double	solve_pl_formula(t_scene *scene, t_pl_ctx *pl_ctx, int flag)
{
	float	diff[3];
	double	t;
	float	hit_p[3];
	float	scale_tD[3];

	if (flag == 1)
		create_iter_pl_ctx(scene, pl_ctx);
	if (fabs(pl_ctx->denom) < 1e-6)
		return (0.0f);
	sub(diff, pl_ctx->p0, pl_ctx->origin);
	t = dot(diff, pl_ctx->normal) / pl_ctx->denom;
	if (t < 0.0f)
		return (-1.0f);
	if (flag == 1 && t < pl_ctx->rec->t)//intersection occurs
	{
		pl_ctx->rec->t = t;
		pl_ctx->rec->hit = 1;
		pl_ctx->rec->object_idx = pl_ctx->i;
		pl_ctx->rec->object_type = PLANE;
		scale(scale_tD, pl_ctx->d, t);			//store into scale_tD = (t*D)
		add(hit_p, pl_ctx->origin, scale_tD);	//hit_p = (O + t * D)
		copy_vectors(pl_ctx->rec->hit_point, hit_p);
		copy_vectors(pl_ctx->rec->normal, pl_ctx->normal);
		copy_int_vectors(pl_ctx->rec->color, pl_ctx->curr_pl->pl_rgb);
	}
	return (t);
}

int	ray_plane_intersect(t_ray_table *ray_table, t_scene *scene)
{
	t_pl_ctx	pl_ctx;
	
	pl_ctx.i = 0;
	while (pl_ctx.i < ray_table->total_rays)
	{
		pl_ctx.d[0] = ray_table->vectors_x[pl_ctx.i];
		pl_ctx.d[1] = ray_table->vectors_y[pl_ctx.i];
		pl_ctx.d[2] = ray_table->vectors_z[pl_ctx.i];
		pl_ctx.rec = &ray_table->hit_record[pl_ctx.i];
		pl_ctx.s = 0;
		while (pl_ctx.s < scene->plane_capacity && scene->plane[pl_ctx.s])
		{
			pl_ctx.curr_pl = scene->plane[pl_ctx.s];
			solve_pl_formula(scene, &pl_ctx, 1);
			pl_ctx.s++;
		}
		pl_ctx.i++;
	}
	return (1);
}

//mlx_pixel_put(win->mlx, win->win, x, y, 0x000000);
void	render_plane_loop(t_ray_table *ray_table, t_window *win)
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

int	render_plane(t_ray_table *ray_table, t_scene *scene, t_window *win)
{
	ray_plane_intersect(ray_table, scene);
	render_plane_loop(ray_table, win);
	mlx_put_image_to_window(win->mlx, win->win, win->img, 0, 0);
	return (1);
}