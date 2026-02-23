/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 01:58:23 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 01:45:13 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_preset	*store_preset(char *line)
{
	t_preset	*preset;
	char		*saveptr;

	preset = calloc(1, sizeof(t_preset));
	if (!preset)
		return (NULL);
	preset->name = ft_strdup(strtok_r(line, "|", &saveptr));
	preset->base = ft_strdup(strtok_r(NULL, "|", &saveptr));
	preset->bump = ft_strdup(strtok_r(NULL, "|", &saveptr));
	if (!check_preset_values(preset))
		return (NULL);
	if (access(preset->base, R_OK) == -1)
	{
		printf("miniRT: error: cannot open texture path <%s>\n", preset->base);
		free_preset(preset);
		return (NULL);
	}
	if (access(preset->bump, R_OK) == -1)
	{
		printf("miniRT: error: cannot open texture path <%s>\n", preset->bump);
		free_preset(preset);
		return (NULL);
	}
	return (preset);
}

void	load_conf_file_loop(t_preset ***preset_list, FILE *f)
{
	size_t	len;
	int		i;
	char	*line;

	line = NULL;
	len = 0;
	i = 0;
	while (i < PRESET_N && getline(&line, &len, f) != -1)
	{
		if (!sanitize_preset_line(*preset_list, line))
			continue ;
		else
			(*preset_list)[i] = store_preset(line);
		if (!(*preset_list)[i])
		{
			clean_preset_list(*preset_list);
			free(line);
			*preset_list = NULL;
			return ;
		}
		i++;
	}
	free(line);
}

t_preset	**load_conf_file(char *filename)
{
	t_preset	**preset_list;
	FILE		*f;

	f = fopen(filename, "r");
	if (!f)
	{
		printf("miniRT: error: failed to open styles.conf\n");
		return (NULL);
	}
	preset_list = calloc(PRESET_N, sizeof(t_preset *));
	if (!preset_list)
	{
		fclose(f);
		return (NULL);
	}
	load_conf_file_loop(&preset_list, f);
	fclose(f);
	return (preset_list);
}

void	print_texture(t_texture *tex)
{
	if (tex->path)
		printf("<%s>\n", tex->path);
	if (tex->preset)
	{
		if (tex->type == BASE)
			printf("<TYPE>: BASE\n");
		if (tex->type == BUMP)
			printf("<TYPE>: BUMP\n");
	}
}

void	handle_right_click(int x, int y, t_app *app)
{
	t_r_click_ctx	ctx;

	if (app->click_lock == 1)
		return ;
	if (!init_right_click_vars(x, y, app, &ctx))
		return ;
	if (!app->preset_list)
	{
		app->preset_list = load_conf_file("configure/styles.conf");
		if (!init_textures(app))
		{
			clean_preset_list(app->preset_list);
			return ;
		}
		app->tex_capacity = PRESET_N * 2;
	}
	ctx.target_preset = choose_preset(app->preset_list);
	if (ctx.target_preset)
		ctx.target_pair = link_texture_preset(ctx.target_preset, app);
	if (ctx.target_pair)
	{
		assign_texture_to_hit_obj(app, ctx.hit, ctx.target_pair);
		free(ctx.target_pair);
	}
	rerender(app);
}
