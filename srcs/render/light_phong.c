/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_phong.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:45:09 by joao-vri          #+#    #+#             */
/*   Updated: 2026/01/24 00:05:09 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	combine_lights(t_render_ctx *render, t_scene *scene, float *dir)
{
	float	obj_color[3];
	float	light_color[3];
	float	diffuse_part[3];
	float	specular_part[3];
	float	diffuse_factor;
	float	spec_factor;
	int		j;

	normalize_target_colors(obj_color, render->rec->color);
	normalize_target_colors(light_color, scene->light->light_rgb);
	diffuse_factor = fmaxf(0.0f, dot(render->rec->normal, dir));
	mult(diffuse_part, obj_color, light_color);
	scale(diffuse_part, diffuse_part,
		scene->light->bright_ratio * diffuse_factor);
	add(render->final_shade, render->final_shade, diffuse_part);
	if (diffuse_factor > 0)
	{
		spec_factor = apply_specular_light(scene, render);
		scale(specular_part, light_color, scene->light->bright_ratio * spec_factor);
		add(render->final_shade, render->final_shade, specular_part);
	}
	j = 0;
	while (j < 3)
	{
		render->final_shade[j] = fmaxf(render->final_shade[j], 0.0f);
		render->final_shade[j] = fminf(render->final_shade[j], 1.0f);
		render->out_shade[j] = (int)(render->final_shade[j] * 255.0f);
		j++;
	}
	return (1);
}

//Color = a * (ca,change / 255) * (co,change / 255)
void	apply_ambient_light(t_scene *scene, t_hit *curr_rec,
	t_render_ctx *render)
{
	float	a;
	float	ca[3];
	float	term;
	int		i;

	a = scene->ambiance->light_ratio;
	ca[0] = (float)scene->ambiance->a_rgb[0];
	ca[1] = (float)scene->ambiance->a_rgb[1];
	ca[2] = (float)scene->ambiance->a_rgb[2];
	render->co[0] = (float)curr_rec->color[0];
	render->co[1] = (float)curr_rec->color[1];
	render->co[2] = (float)curr_rec->color[2];
	normalize_colors(ca);
	normalize_colors(render->co);
	i = 0;
	while (i < 3)
	{
		term = a * ca[i] * render->co[i];
		term = fmaxf(term, 0.0f);
		term = fminf(term, 1.0f);
		render->final_shade[i] = term;
		render->a_shade[i] = (int)(term * 255.0f);
		i++;
	}
}

float	apply_specular_light(t_scene *scene, t_render_ctx *render)
{
	float	r_vector[3];
	float	l_vector[3];
	float	v_vector[3];
	double	dot_res;
	double	spec_angle;

	sub(v_vector, scene->camera->coordinates_xyz, render->rec->hit_point);
	normalize(v_vector, v_vector);
	sub(l_vector, scene->light->light_xyz, render->rec->hit_point);
	normalize(l_vector, l_vector);
	dot_res = dot(render->rec->normal, l_vector);
	dot_res *= 2;
	scale(r_vector, render->rec->normal, dot_res);
	sub(r_vector, r_vector, l_vector);
	spec_angle = dot(r_vector, v_vector);
	spec_angle = fmax(0.0, spec_angle);
	spec_angle = pow(spec_angle, 50);
	return (spec_angle);
}
