/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:23:47 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 01:45:55 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	close_window(t_app *app)
{
	cleanup_all(app->scene);
	free(app->ray_table->vectors_x);
	free(app->ray_table->vectors_y);
	free(app->ray_table->vectors_z);
	if (app->textures)
		clean_textures_list(app ,app->textures);
	if (app->preset_list)
		clean_preset_list(app->preset_list);
	if (app->ray_table->hit_record)
		free (app->ray_table->hit_record);
	if (app->win->win)
		mlx_destroy_window(app->win->mlx, app->win->win);
	if (app->win->img)
		mlx_destroy_image(app->win->mlx, app->win->img);
	if (app->win->mlx)
	{
		mlx_destroy_display(app->win->mlx);
		free(app->win->mlx);
	}
	exit(0);
	return (0);
}

int	key_press(int keycode, t_app *app)
{
	if (keycode == KEY_ESC)
		close_window(app);
	return (0);
}

int	init_right_click_vars(int x, int y, t_app *app, t_r_click_ctx *ctx)
{
	ctx->target_pair = NULL;
	ctx->target_preset = NULL;
	ctx->index = 0;
	ctx->hit = NULL;
	ctx->index = y * app->win->width + x;
	ctx->hit = &app->ray_table->hit_record[ctx->index];
	if (!ctx->hit->hit)
		return (0);
	return (1);
}

int	mouse_click_handler(int keycode, int x, int y, t_app *app)
{
	long	first_click;

	first_click = get_time_ms();
	if (app->click_lock == 1)
		return (0);
	if (keycode == 1)
	{
		printf("Left click at (%d, %d) at %ldms\n", x, y, first_click);
		handle_click(x, y, app);
	}
	else if (keycode == 3)
	{
		printf("Right click at (%d, %d)\n", x, y);
		handle_right_click(x, y, app);
		app->click_lock = 0;
	}
	return (0);
}
