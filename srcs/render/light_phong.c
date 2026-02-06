/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_phong.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:45:09 by joao-vri          #+#    #+#             */
/*   Updated: 2026/02/06 21:31:02 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	combine_lights(t_render_ctx *render, t_scene *scene, float *dir)
{
	t_light_model_ctx	ctx;

	normalize_target_colors(ctx.obj_color, render->rec->color);
	normalize_target_colors(ctx.light_color, scene->light->light_rgb);
	ctx.diffuse_factor = fmaxf(0.0f, dot(render->rec->normal, dir));
	mult(ctx.diffuse_part, ctx.obj_color, ctx.light_color);
	scale(ctx.diffuse_part, ctx.diffuse_part,
		scene->light->bright_ratio * ctx.diffuse_factor);
	add(render->final_shade, render->final_shade, ctx.diffuse_part);
	if (ctx.diffuse_factor > 0)
	{
		ctx.spec_factor = apply_specular_light(scene, render);
		scale(ctx.specular_part, ctx.light_color, scene->light->bright_ratio
			* ctx.spec_factor);
		add(render->final_shade, render->final_shade, ctx.specular_part);
	}
	ctx.j = 0;
	while (ctx.j < 3)
	{
		render->final_shade[ctx.j] = fmaxf(render->final_shade[ctx.j], 0.0f);
		render->final_shade[ctx.j] = fminf(render->final_shade[ctx.j], 1.0f);
		render->out_shade[ctx.j] = (int)(render->final_shade[ctx.j] * 255.0f);
		ctx.j++;
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
