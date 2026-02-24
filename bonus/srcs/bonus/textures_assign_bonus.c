/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_assign.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:14:55 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/20 11:02:11 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

t_tex_pair	*link_texture_preset(t_preset *preset, t_app *app)
{
	int			i;
	t_tex_pair	*pair;

	pair = calloc(1, sizeof(t_tex_pair));
	if (!pair)
		return (NULL);
	i = 0;
	while (i < app->tex_capacity)
	{
		if (app->textures[i] && app->textures[i]->type == BASE
			&& strcmp(app->textures[i]->preset->name, preset->name) == 0)
			pair->base = app->textures[i];
		if (app->textures[i] && app->textures[i]->type == BUMP
			&& strcmp(app->textures[i]->preset->name, preset->name) == 0)
			pair->bump = app->textures[i];
		i++;
	}
	if (!pair->base || !pair->bump)
	{
		pair->base = NULL;
		pair->bump = NULL;
		free(pair);
		return (NULL);
	}
	return (pair);
}

t_preset	*input_match_preset_name(t_preset **list, char *line)
{
	int	i;

	i = 0;
	while (i < PRESET_N)
	{
		if (strcmp(list[i]->name, line) == 0)
		{
			return (list[i]);
		}
		i++;
	}
	return (NULL);
}

void	assign_texture_to_hit_obj(t_app *app, t_hit *hit, t_tex_pair *pair)
{
	if (hit->object_type == SPHERE)
	{
		if (app->scene->sphere[hit->obj_scene_idx] != NULL)
		{
			app->scene->sphere[hit->obj_scene_idx]->base = pair->base;
			app->scene->sphere[hit->obj_scene_idx]->bump = pair->bump;
		}
	}
	else if (hit->object_type == PLANE)
	{
		if (app->scene->plane[hit->obj_scene_idx] != NULL)
		{
			app->scene->plane[hit->obj_scene_idx]->base = pair->base;
			app->scene->plane[hit->obj_scene_idx]->bump = pair->bump;
		}
	}
	else if (hit->object_type == CYLINDER)
	{
		if (app->scene->cylinder[hit->obj_scene_idx] != NULL)
		{
			app->scene->cylinder[hit->obj_scene_idx]->base = pair->base;
			app->scene->cylinder[hit->obj_scene_idx]->bump = pair->bump;
		}
	}
}

void	print_available_presets(t_preset **list)
{
	int	i;

	i = 0;
	printf("Type to choose the available textures for clicked object:\n");
	while (i < PRESET_N)
	{
		printf("\t[%d]%s", i + 1, list[i]->name);
		i++;
	}
	printf("\n");
}

t_preset	*choose_preset(t_preset **list)
{
	char		*line;
	size_t		len;
	t_preset	*res;
	char		*trimmed;

	line = NULL;
	res = NULL;
	len = 0;
	if (!list)
		return (NULL);
	print_available_presets(list);
	tcflush(STDIN_FILENO, TCIFLUSH);
	while (getline(&line, &len, stdin) != -1)
	{
		trimmed = ft_strtrim(line, "\n\t\b ");
		printf("you typed: <%s>\n", trimmed);
		res = input_match_preset_name(list, trimmed);
		if (res)
			return (free(line), free(trimmed), res);
		free(trimmed);
		tcflush(STDIN_FILENO, TCIFLUSH);
	}
	free (line);
	return (res);
}
