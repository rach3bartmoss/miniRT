/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   click_event_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 11:36:25 by dopereir          #+#    #+#             */
/*   Updated: 2026/01/27 23:35:00 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	rerender(t_app *app, t_hit *hit)
{
	(void)hit;
	if (app->click_lock == 1)
		return ;
	app->click_lock = 1;
	if (app->win->img)
		mlx_destroy_image(app->win->mlx, app->win->img);
	app->win->img = mlx_new_image(app->win->mlx, app->win->width,
			app->win->height);
	app->win->addr = mlx_get_data_addr(app->win->img, &app->win->bpp,
			&app->win->line_length, &app->win->endian);
	start_multithread_render(app);
	app->click_lock = 0;
}

static void	validate_idx_checkerboard(t_app *app, t_hit *hit)
{
	if (hit->object_type == SPHERE)
	{
		if (hit->obj_scene_idx < app->scene->sphere_capacity
			&& app->scene->sphere[hit->obj_scene_idx] != NULL)
			app->scene->sphere[hit->obj_scene_idx]->checkerboard ^= 1;
	}
	else if (hit->object_type == PLANE)
	{
		if (hit->obj_scene_idx < app->scene->plane_capacity
			&& app->scene->plane[hit->obj_scene_idx] != NULL)
			app->scene->plane[hit->obj_scene_idx]->checkerboard ^= 1;
	}
	else if (hit->object_type == CYLINDER)
	{
		if (hit->obj_scene_idx < app->scene->cylinder_capacity
			&& app->scene->cylinder[hit->obj_scene_idx] != NULL)
			app->scene->cylinder[hit->obj_scene_idx]->checkerboard ^= 1;
	}
}

void	handle_click(int x, int y, t_app *app)
{
	int		index;
	t_hit	*hit;

	if (app->click_lock == 1)
		return ;
	index = y * app->win->width + x;
	hit = &app->ray_table->hit_record[index];
	if (!hit->hit)
		return ;
	validate_idx_checkerboard(app, hit);
	rerender(app, hit);
}

/*	--PRINTS FOR DEBUG
	printf("hit->hit = %d\n", hit->hit);
	printf("hit->object_type = %d\n", hit->object_type);
	printf("hit->object_idx = %d\n", hit->object_idx);
	printf("hit->OBJ_SCENE_IDX = %d\n", hit->obj_scene_idx);

	printf("sphere_capacity = %d\n", app->scene->sphere_capacity);
	printf("plane_capacity = %d\n", app->scene->plane_capacity);
	printf("cylinder_capacity = %d\n", app->scene->cylinder_capacity);
*/
