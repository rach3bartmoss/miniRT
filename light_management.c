/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:01:03 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/30 23:49:13 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	normalize_colors(float rgb[3])
{
	rgb[0] /= 255.0f;
	rgb[1] /= 255.0f;
	rgb[2] /= 255.0f;
}

double	ray_length(float vector[3])
{
	double	xyz;
	
	xyz = (double)vector[0] * vector[0];
	xyz += (double)vector[1] * vector[1];
	xyz += (double)vector[2] * vector[2];
	return (sqrt(xyz));
}

//Color = a * (ca,change / 255) * (co,change / 255)
void	apply_ambient_light(t_scene *scene, t_hit *curr_rec, t_render_ctx *render)
{
	float	a;
	float	ca[3];
	float	co[3];
	float	term;
	int		i;

	a = scene->ambiance->light_ratio;
	ca[0] = (float)scene->ambiance->a_rgb[0];
	ca[1] = (float)scene->ambiance->a_rgb[1];
	ca[2] = (float)scene->ambiance->a_rgb[2];
	co[0] = (float)curr_rec->color[0];
	co[1] = (float)curr_rec->color[1];
	co[2] = (float)curr_rec->color[2];
	normalize_colors(ca);
	normalize_colors(co);
	i = 0;
	while (i < 3)
	{
		term = a * ca[i] * co[i];
		term = fmaxf(term, 0.0f);
		term = fminf(term, 1.0f);
		render->final_shade[i] = term;
		render->a_shade[i] = (int)(term * 255.0f);
		i++;
	}
}

int	is_occluded(t_scene *scene, float origin[3], float dir[3], float max_t)
{
	double		t;
	t_sp_ctx	sp_ctx;


	for (int si = 0;si < scene->sphere_capacity && scene->sphere[si]; ++si)
	{
		sp_ctx.curr_sp = scene->sphere[si];
		t = solve_discriminant(origin, dir, &sp_ctx, 0);
		if (t > SHADOW_EPS && t < max_t)
			return (1);
	}
	return (0);
}

//shadow ray = R(t) = O + tDir
float	diffuse_and_shadow_algo(t_ray_table *ray_table, t_scene *scene, t_render_ctx *render)
{
	float	light_dir[3];
	float	dist_to_light;
	float	offset[3];
	float	origin[3];

	(void)ray_table;
	sub(light_dir, scene->light->light_xyz, render->rec->hit_point);
	dist_to_light = ray_length(light_dir);
	normalize(light_dir, light_dir);

	scale(offset, render->rec->normal, SHADOW_EPS);
	add(origin, render->rec->hit_point, offset);
	
	if (is_occluded(scene, origin, light_dir, dist_to_light))
		return (0.0);

	//Lambertian diffuse: cos = max(0, N*light_light)
	float	n_dot_l;

	n_dot_l = dot(render->rec->normal, light_dir);
	if (n_dot_l <= 0.0f)
		return 0.0f;

	// weight = brightness * cosθ
	return scene->light->bright_ratio * n_dot_l;
	
	/*n_dot_l = fmaxf(0.0f ,dot(ray_table->hit_record[render->i].normal, light_dir));
	
	lc[0] = scene->light->light_rgb[0] / 255.0f;
	lc[1] = scene->light->light_rgb[1] / 255.0f;
	lc[2] = scene->light->light_rgb[2] / 255.0f;

	render->c_obj_term[0] = render->rec->color[0] / 255.0f;
	render->c_obj_term[1] = render->rec->color[1] / 255.0f;
	render->c_obj_term[2] = render->rec->color[2] / 255.0f;

	diff = scene->light->bright_ratio * n_dot_l;

	for (int c = 0; c < 3; ++c)
	{
		render->a_shade[c] += diff * lc[c] * render->c_obj_term[c];
	}
	for (int c = 0; c < 3; ++c)
	{
		render->a_shade[c] = fminf(render->a_shade[c], 1.0f);
	}
	return (0.0);
	
	double	res = (double)scene->light->bright_ratio * n_dot_l;
	return(res);*/
}
