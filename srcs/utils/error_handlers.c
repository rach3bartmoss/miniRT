/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 20:00:04 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/06 21:18:29 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/// @brief Prints error messages if open() fails. 
/// @param filename Name of the target file
/// @param errno_code errno from <errno.h> library
int	open_error_cases(char *filename, int errno_code)
{
	(void) filename;
	if (errno_code == EACCES)
	{
		printf("MiniRT: '%s'\n", strerror(errno_code));
		return (-1);
	}
	if (errno_code == ENOENT)
	{
		printf("MiniRT: '%s'\n", strerror(errno_code));
		return (-1);
	}
	if (errno_code == ENAMETOOLONG)
	{
		printf("MiniRT: Name of the file too long.\n");
		return (-1);
	}
	printf("MiniRT: Unexpected error: '%s'.\n", strerror(errno_code));
	return (-1);
}

/*void	print_element(void *elem, e_type_elem type)
{
	if (!elem)
	{
		printf("Element is NULL\n");
		return;
	}

	switch (type)
	{
		case AMBIENT: {
			t_ambient *a = (t_ambient *)elem;
			printf("Ambient [id: %c]\n", a->id);
			printf("	Light Ratio: %.2f\n", a->light_ratio);
			printf("	RGB: [%d, %d, %d]\n", a->a_rgb[0], a->a_rgb[1],
				a->a_rgb[2]);
			break;
		}
		case CAMERA: {
			t_camera *c = (t_camera *)elem;
			printf("Camera [id: %c]\n", c->id);
			printf("	Coordinates: [%.2f, %.2f, %.2f]\n",
				c->coordinates_xyz[0],
				c->coordinates_xyz[1],
				c->coordinates_xyz[2]);
			printf("	Orientation Vector: [%.2f, %.2f, %.2f]\n",
				c->vector_xyz[0],
				c->vector_xyz[1],
				c->vector_xyz[2]);
			printf("	FOV: %f\n", c->fov);
			break;
		}
		case LIGHT: {
			t_light *l = (t_light *)elem;
			printf("Light [id: %c]\n", l->id);
			printf("	Position: [%.2f, %.2f, %.2f]\n", l->light_xyz[0],
				l->light_xyz[1], l->light_xyz[2]);
			printf("	Brightness: %.2f\n", l->bright_ratio);
			printf("	RGB: [%d, %d, %d]\n", l->light_rgb[0],
				l->light_rgb[1], l->light_rgb[2]);
			break;
		}
		case SPHERE: {
			t_sphere *s = (t_sphere *)elem;
			printf("Sphere [id: %s]\n", s->id);
			printf("	Center: [%.2f, %.2f, %.2f]\n",
				s->sp_center_xyz[0],
				s->sp_center_xyz[1],
				s->sp_center_xyz[2]);
			printf("	Diameter: %.2f\n", s->sp_diameter);
			printf("	RGB: [%d, %d, %d]\n",
				s->sp_rgb[0],
				s->sp_rgb[1],
				s->sp_rgb[2]);
			break;
		}
		case PLANE: {
			t_plane *p = (t_plane *)elem;
			printf("Plane [id: %s]\n", p->id);
			printf("	Point: [%.2f, %.2f, %.2f]\n",
				p->pl_xyz[0], p->pl_xyz[1], p->pl_xyz[2]);
			printf("	Vector: [%.2f, %.2f, %.2f]\n",
				p->pl_vector_xyz[0],
				p->pl_vector_xyz[1],
				p->pl_vector_xyz[2]);
			printf("	RGB: [%d, %d, %d]\n",
				p->pl_rgb[0],
				p->pl_rgb[1],
				p->pl_rgb[2]);
			break;
		}
		case CYLINDER: {
			t_cylinder *cy = (t_cylinder *)elem;
			printf("Cylinder [id: %s]\n", cy->id);
			printf("	Center: [%.2f, %.2f, %.2f]\n",
				cy->cy_xyz[0],
				cy->cy_xyz[1],
				cy->cy_xyz[2]);
			printf("	Vector: [%.2f, %.2f, %.2f]\n",
				cy->cy_vector_xyz[0],
				cy->cy_vector_xyz[1],
				cy->cy_vector_xyz[2]);
			printf("	Diameter: %.2f\n", cy->cy_diameter);
			printf("	Height: %.2f\n", cy->cy_height);
			printf("	RGB: [%d, %d, %d]\n",
				cy->cy_rgb[0],
				cy->cy_rgb[1],
				cy->cy_rgb[2]);
			break;
		}
		case PARABOLOID: {
			t_paraboloid *pa = (t_paraboloid *)elem;
			printf("Paraboloid [id: %s]\n", pa->id);
			printf("	Center: [%.2f, %.2f, %.2f]\n",
			pa->center[0],
			pa->center[1],
			pa->center[2]);
			printf("	Axis: [%.2f, %.2f, %.2f]\n",
				pa->axis[0],
				pa->axis[1],
				pa->axis[2]);
			printf("	K (Steepnees): %.2f\n", pa->k);
			printf("	Height: %.2f\n", pa->height);
			printf("	RGB: [%d, %d, %d]\n",
				pa->rgb[0], pa->rgb[1], pa->rgb[2]);
			break;
		}
		default:
			printf("Unknown element type\n");
	}
}

void	print_spheres(t_scene *scene)
{
	int	count = set_and_get_occ(-1, SPHERE);

	for (int i = 0; i < count; i++)
	{
		if (scene->sphere[i])
			print_element(scene->sphere[i], SPHERE);
	}
}

void	print_planes(t_scene *scene)
{
	int	count = set_and_get_occ(-1, PLANE);

	for (int i = 0; i < count; i++)
	{
		if (scene->plane[i])
			print_element(scene->plane[i], PLANE);
	}
}

void	print_cylinders(t_scene *scene)
{
	int	count = set_and_get_occ(-1, CYLINDER);

	for (int i = 0; i < count; i++)
	{
		if (scene->cylinder[i])
			print_element(scene->cylinder[i], CYLINDER);
	}
}

void	print_paraboloid(t_scene *scene)
{
	int	count = set_and_get_occ(-1, PARABOLOID);

	for (int i = 0; i < count; i++)
	{
		if (scene->paraboloid[i])
			print_element(scene->paraboloid[i], PARABOLOID);
	}
}*/
