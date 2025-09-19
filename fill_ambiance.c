/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_ambiance.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 23:14:40 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/19 22:22:21 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief Check if the current parsed line has extras whitespaces
/// @brief at the end, or if have another element, in case of a
/// @brief another element we return error code.
/// @param raw_line Current parsed line.
/// @return 
int	check_whitespaces_after_parse(char *raw_line)
{
	char	*token;
	int		skip;

	if (!raw_line)
		return (0);
	skip = 0;
	token = ft_strtok(raw_line, " \t\v\f\r\n");
	while (token != NULL && skip < 3)
	{
		skip++;
		token = ft_strtok(NULL, " \t\v\f\r\n");
	}
	while (token != NULL)
	{
		printf("<%s.>\n", token);
		if (*token != '\0' && ft_strspn(token, " \t\v\f\r") != ft_strlen(token))
			return (0);
		token = ft_strtok(NULL, " \t\v\f\r");
	}
	return (1);
}

int	pre_ambiance_validation(t_scene *scene, char *line, char **parse_line)
{
	*parse_line = ft_strdup(line);
	if (!*parse_line)
		return (0);
	scene->ambiance = malloc(sizeof(t_ambient));
	if (!scene->ambiance)
	{
		free (*parse_line);
		return (0);
	}
	scene->ambiance->id = 'A';
	return (1);
}

int	fill_ambiance_helper(t_scene *scene, char *parse_line, int *rc)
{
	char	*token;
	int		token_count;

	*rc = 1;
	token = ft_strtok(parse_line, " \t\v\f\r");
	token_count = 0;
	while (token != NULL && token_count < 3)
	{
		if (token_count == 1)
			*rc = parse_light_ratio(token, &scene->ambiance->light_ratio, AMBIENT);
		if (token_count == 2 && *rc == 1)
			*rc = parse_rgb(token, scene->ambiance->a_rgb, AMBIENT);
		if (*rc != 1)
			return (0);
		token_count++;
		token = ft_strtok(NULL, " \t\v\f\r");
	}
	return (1);
}

int	fill_ambiance(t_scene *scene, char *line)
{
	char	*parse_line;
	char	*post_parse_line;
	int		rc;

	if (set_and_get_occ(GET_OCCURRENCE, AMBIENT) > 0)
	{
		printf("miniRT: the input file has more than one definition of ambient light\n");
		return (0);
	}
	parse_line = NULL;
	if (!pre_ambiance_validation(scene, line, &parse_line))
		return (0);
	if (fill_ambiance_helper(scene, parse_line, &rc) != 1)
		return (handle_default_error((void **)&scene->ambiance,
				parse_line, NULL));
	post_parse_line = ft_strdup(line);
	rc = check_whitespaces_after_parse(post_parse_line);
	if (rc != 1)
		return (handle_default_error((void **)&scene->ambiance,
				parse_line, post_parse_line));
	free (post_parse_line);
	free (parse_line);
	set_and_get_occ(1, AMBIENT);
	return (1);
}
