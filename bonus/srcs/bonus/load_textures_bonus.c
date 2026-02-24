/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 22:16:39 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/24 03:31:52 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

t_texture	*load_xpm_file(t_app *app, char *filename)
{
	t_texture	*new_tex;
	void		*img;

	new_tex = ft_calloc(1, sizeof(t_texture));
	if (!new_tex)
		return (NULL);
	img = mlx_xpm_file_to_image(app->win->mlx, filename, &new_tex->width,
			&new_tex->height);
	if (!img)
	{
		free(new_tex);
		return (NULL);
	}
	new_tex->buffer = mlx_get_data_addr(img, &new_tex->bpp,
			&new_tex->line_len, &new_tex->endian);
	new_tex->path = ft_strdup(filename);
	if (!new_tex->path)
	{
		mlx_destroy_image(app->win->mlx, img);
		free(new_tex);
		return (NULL);
	}
	new_tex->img = img;
	return (new_tex);
}

/// @brief checks if the texture is already loaded
/// @param app store the texture in app->textures
/// @param filename 
/// @return returns a pointer to the loaded texture
t_texture	*load_texture(t_app *app, char *filename)
{
	t_texture	*new_tex;
	int			i;

	i = 0;
	while (i < app->n_textures)
	{
		if (app->textures[i])
			if (strcmp(filename, app->textures[i]->path) == 0)
				return (app->textures[i]);
		i++;
	}
	new_tex = load_xpm_file(app, filename);
	if (!new_tex)
		return (NULL);
	return (new_tex);
}

void	init_textures_assing_loop(t_app *app, int *i, int *j)
{
	if (app->preset_list[*j])
		app->textures[*i] = load_texture(app, app->preset_list[*j]->base);
	if (app->textures[*i])
	{
		app->textures[*i]->type = BASE;
		app->textures[*i]->preset = app->preset_list[*j];
	}
	(*i)++;
	if (app->preset_list[*j])
		app->textures[*i] = load_texture(app, app->preset_list[*j]->bump);
	if (app->textures[*i])
	{
		app->textures[*i]->type = BUMP;
		app->textures[*i]->preset = app->preset_list[*j];
	}
	(*j)++;
	(*i)++;
}

int	init_textures(t_app *app)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	app->tex_capacity = PRESET_N * 2;
	app->textures = calloc(app->tex_capacity + 1, sizeof(t_texture *));
	if (!app->textures)
		return (printf("miniRT: error: init_textures failed.\n"), 0);
	while (i < app->tex_capacity)
	{
		init_textures_assing_loop(app, &i, &j);
	}
	app->textures[app->tex_capacity] = NULL;
	return (1);
}
