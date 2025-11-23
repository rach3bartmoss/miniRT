/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:23:47 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/25 14:30:26 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	close_window(t_app *app)
{
	cleanup_all(app->scene);
	free(app->ray_table->vectors_x);
	free(app->ray_table->vectors_y);
	free(app->ray_table->vectors_z);
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
