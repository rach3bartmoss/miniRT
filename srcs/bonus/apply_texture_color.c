/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_texture_color.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:28:46 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/21 12:59:46 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	validate_obj_texture(e_type_elem type, int obj_idx, t_app *app)
{
	if (type == SPHERE)
	{
		if (app->scene->sphere[obj_idx]->base)
			return (1);
	}
	if (type == PLANE)
	{
		if (app->scene->plane[obj_idx]->base)
			return (1);
	}
	return (0);
}

void	get_texture_color(t_texture *tex, int x, int y, int color[3])
{
	unsigned int	pixel;
	int				offset;

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x >= tex->width)
		x = tex->width - 1;
	if (y >= tex->height)
		y = tex->height - 1;
	offset = y * tex->line_len + x * (tex->bpp / 8);
	pixel = *(unsigned int *)(tex->buffer + offset);
	color[0] = (pixel >> 16) & 0xFF;
	color[1] = (pixel >> 8) & 0xFF;
	color[2] = pixel & 0xFF;
}

void	apply_sphere_texture(t_hit *hit, t_texture *tex)
{
	float	n[3];
	float	uv[2];
	int		xy[2];

	copy_vectors(n, hit->normal);
	uv[0] = 0.5f + atan2f(n[2], n[0]) / (2.0f * M_PI);
	uv[1] = 0.5f - asinf(n[1]) / M_PI;
	xy[0] = (int)(uv[0] * tex->width);
	xy[1] = (int)(uv[1] * tex->height);
	if (xy[0] >= tex->width)
		xy[0] = tex->width - 1;
	if (xy[1] >= tex->height)
		xy[1] = tex->height - 1;
	get_texture_color(tex, xy[0], xy[1], hit->color);
}

int	apply_texture_colors(t_app *app)
{
	int			i;
	int			obj_idx;
	e_type_elem	type;
	t_hit		*hit;
	t_texture	*tex;

	i = 0;
	while (i < app->ray_table->total_rays)
	{
		obj_idx = app->ray_table->hit_record[i].obj_scene_idx;
		type = app->ray_table->hit_record[i].object_type;
		if (validate_obj_texture(type, obj_idx, app))
		{
			hit = &app->ray_table->hit_record[i];
			if (!hit->hit)
			{
				i++;
				continue ;
			}
			if (type == SPHERE)
			{
				tex = app->scene->sphere[obj_idx]->base;
				apply_sphere_texture(hit, tex);
				//tex = app->scene->sphere[obj_idx]->bump;
				//apply_sphere_bump(hit, tex);
			}
			if (type == PLANE)
			{
				tex = app->scene->plane[obj_idx]->base;
				apply_plane_texture(hit, tex, app->scene->plane[obj_idx]);
			}
		}
		i++;
	}
	return (0);
}