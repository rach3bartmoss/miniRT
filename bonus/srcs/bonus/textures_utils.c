/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 01:34:39 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/20 00:07:12 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	clean_textures_list(t_app *app, t_texture **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (i < TEXTURE_N)
	{
		if (arr[i])
		{
			if (arr[i]->path)
				free(arr[i]->path);
			if (arr[i]->img)
				mlx_destroy_image(app->win->mlx, arr[i]->img);
			arr[i]->preset = NULL;
			free(arr[i]);
		}
		i++;
	}
	free(arr);
}

void	clean_preset_list(t_preset **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (i < PRESET_N)
	{
		if (arr[i])
			free_preset(arr[i]);
		i++;
	}
	free(arr);
}

void	free_preset(t_preset *preset)
{
	if (!preset)
		return ;
	if (preset->name)
		free(preset->name);
	if (preset->base)
		free(preset->base);
	if (preset->bump)
		free(preset->bump);
	if (preset)
		free(preset);
}

int	check_preset_values(t_preset *preset)
{
	if (!preset->name || !preset->base || !preset->bump)
	{
		free_preset(preset);
		return (0);
	}
	return (1);
}

int	sanitize_preset_line(t_preset **list, char *line)
{
	int		i;
	char	*name;
	char	*saveptr;
	char	*tmp;

	i = 0;
	while (line[i] && !ft_isalpha(line[i]))
	{
		if (line[i] == '#')
			return (0);
		i++;
	}
	tmp = ft_strdup(line);
	name = strtok_r(tmp, "|", &saveptr);
	i = 0;
	while (list[i])
	{
		if (strcmp(list[i]->name, name) == 0)
			return (free(tmp), 0);
		i++;
	}
	return (free(tmp), 1);
}
