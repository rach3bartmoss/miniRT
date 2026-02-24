/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 13:59:07 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 23:37:20 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	apply_object_base_color(t_render_ctx *render, t_scene *scene)
{
	int	type;
	int	idx;

	type = render->rec->object_type;
	idx = render->rec->obj_scene_idx;
	if (type == PLANE && scene->plane[idx]->checkerboard)
		apply_checkerboard_for_plane(render->rec,
			scene->plane[idx], render->rec->color);
	else if (type == SPHERE && scene->sphere[idx]->checkerboard)
		apply_checkboard_for_sphere(render->rec,
			scene->sphere[idx], render->rec->color);
	else if (type == CYLINDER && scene->cylinder[idx]->checkerboard)
		apply_checkerboard_cy(render->rec, scene->cylinder[idx],
			render->rec->color);
	else
		reverse_checkboard_pattern(render, scene);
}

static void	intersect_objects(t_app *app)
{
	ray_sphere_intersect(app->ray_table, app->scene);
	ray_plane_intersect(app->ray_table, app->scene);
	ray_cylinder_intersection(app->ray_table, app->scene);
	ray_paraboloid_intersection(app->ray_table, app->scene);
}

void	render_objects(t_app *app)
{
	t_hit	*rec;
	int		color;
	int		x;
	int		y;
	int		i;

	intersect_objects(app);
	y = 0;
	while (y < app->win->height)
	{
		x = 0;
		while (x < app->win->width)
		{
			i = y * app->win->width + x;
			rec = &app->ray_table->hit_record[i];
			if (!rec->hit)
				color = 0x000000;
			else
				color = apply_shade_to_pixel(app, rec);
			mrt_put_pixel(app->win, x, y, color);
			x++;
		}
		y++;
	}
}
