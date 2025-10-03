/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:01:03 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/04 00:51:19 by dopereir         ###   ########.fr       */
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
		render->final_shade[i] = term;//save as a float [0,1]
		render->a_shade[i] = (int)(term * 255.0f); //save as int 255
		i++;
	}
}

float	ray_intersection_sp_tester(float *sr_origin, float *sr_dir, t_sphere *sphere)
{
	t_sp_ctx	sp_ctx;
	float		t;

	sp_ctx.curr_sp = sphere;
	t = solve_discriminant(sr_origin, sr_dir, &sp_ctx, 0);
	return (t);
}

int	apply_diffuse_and_shadow(t_render_ctx *render, t_scene *scene, t_window *win)
{
	(void)win;
	float	light_dir[3];
	float	light_dir_normalized[3]; //ACTUAL SHADOW RAY ORIGIN NORMALIZED
	float	p_origin[3]; //origem do ray (render->rec->hitpoint)
	float	p_offset[3]; //ACTUAL SHADOW RAY ORIGIN
	float	ntimes_epsilon[3]; // (N (surface) * 1e-4)
	float	distance;

	//1 PART - get ray origin and direction
	sub(light_dir, scene->light->light_xyz, render->rec->hit_point);
	normalize(light_dir, light_dir_normalized);
	copy_vectors(p_origin, render->rec->hit_point);
	scale(ntimes_epsilon, render->rec->normal, SHADOW_EPS);//gets (N * ε)
	add(p_offset, p_origin, ntimes_epsilon);

	//2 part - test intersections
	float	l_pos_p[3]; //(Light pos - P_origin)
	sub(l_pos_p, scene->light->light_xyz, render->rec->hit_point);
	distance = ray_length(l_pos_p);//distance from hit_point to light point

	double	t;
	int		is_in_shadow = 0;
	int	sp_count = set_and_get_occ(-1, SPHERE);
	int	i = 0;
	while (i < sp_count)
	{
		t = ray_intersection_sp_tester(p_offset, light_dir_normalized, scene->sphere[i]);
		if (t > 0.0f && t < distance)//intersection happens
		{
			is_in_shadow = 1;
			break ;
		}
		i++;
	}

	//3 part - calculate final color
	float	diffuse_part[3];
	float	diffuse_factor;
	float	obj_color[3];
	float	light_color[3];

	
	
	if (is_in_shadow == 0)
	{
		for (int j = 0;i<3;i++)
		{
			obj_color[j]   = render->rec->color[j] / 255.0f;
			light_color[j] = scene->light->light_rgb[j] / 255.0f;
		}
		
		diffuse_factor = fmaxf(0.0f, dot(render->rec->normal, light_dir_normalized));
		
		diffuse_part[0] = obj_color[0] * light_color[0];
		diffuse_part[1] = obj_color[1] * light_color[1];
		diffuse_part[2] = obj_color[2] * light_color[2];

		diffuse_part[0] *= (scene->light->bright_ratio * diffuse_factor);
		diffuse_part[1] *= (scene->light->bright_ratio * diffuse_factor);
		diffuse_part[2] *= (scene->light->bright_ratio * diffuse_factor);

		render->final_shade[0] += diffuse_part[0];
		render->final_shade[1] += diffuse_part[1];
		render->final_shade[2] += diffuse_part[2];

		for (int i = 0; i < 3; i++)
		{
			render->final_shade[i] = fmaxf(render->final_shade[i], 0.0f);
			render->final_shade[i] = fminf(render->final_shade[i], 1.0f);
			render->out_shade[i] = (int)(render->final_shade[i] * 255.0f);
		}
		return (1);
	}
	return (0);
}
