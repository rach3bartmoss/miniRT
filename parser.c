/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ginfranc <ginfranc@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 18:48:14 by dopereir          #+#    #+#             */
/*   Updated: 2025/11/22 14:02:18 by ginfranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief Checks if .rt description file is valid.
/// @param filename Name of the .rt file.
/// @return return 0 on error, 1 on success.
int	check_filename(char *filename)
{
	int	len;

	if (!filename)
		return (0);
	len = ft_strlen(filename);
	if (len < 3)
		return (0);
	if (ft_strcmp(filename + len -3, ".rt") == 0)
		return (1);
	return (0);
}

int	file_management(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (open_error_cases(filename, errno));
	return (fd);
}

int	parse_file(t_scene	*scene, char *filename)
{
	char	*line;
	int		rc;
	int		fd;

	rc = 1;
	fd = file_management(filename);
	if (fd < 0)
		return (0);
	if (!init_objects(scene))
	{
		close(fd);
		return (0);
	}
	line = get_next_line(fd);
	while (line != NULL)
	{
		rc = tokenize_line(line, scene);
		if (rc != 1)
		{
			sanitize_gnl(fd);
			close(fd);
			free(line);
			return (rc);
		}
		free (line);
		line = get_next_line(fd);
	}
	close(fd);
	return (rc);
}
