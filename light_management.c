/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:01:03 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/06 22:29:53 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

	double	t_sp;
	double	t_pl;
	int		is_in_shadow = 0;
	int		sp_count = set_and_get_occ(-1, SPHERE);
	int	i = 0;
	while (i < sp_count)
	{
		t_sp = ray_intersection_sp(p_offset, light_dir_normalized, scene->sphere[i]);
		if (t_sp > 0.0f && t_sp < distance)//intersection happens
		{
			is_in_shadow = 1;
			break ;
		}
		i++;
	}

	i = 0;
	
	if (!is_in_shadow)
	{
		int	pl_count = set_and_get_occ(-1, PLANE);
		for (int p = 0; p < pl_count; ++p)
		{
			t_pl = ray_intersection_pl(p_offset, light_dir_normalized, scene->plane[i]);
			if (t_pl > 0.0f && t_pl < distance)
			{
				is_in_shadow = 1;
				break ;
			}
		}
	}

	i = 0;
	double	t_cy;
	if (!is_in_shadow)
	{
		int	cy_count = set_and_get_occ(-1, CYLINDER);
		for (int c = 0; c < cy_count; ++c)
		{
			t_cy = ray_intersection_cy(p_offset, light_dir_normalized, scene->cylinder[c], distance);
			if (t_cy > 0.0f && t_cy < distance)
			{
				is_in_shadow = 1;
				break ;
			}
		}
	}

	//3 part - calculate final color
	float	diffuse_part[3];
	float	diffuse_factor;
	float	obj_color[3];
	float	light_color[3];

	
	
	if (!is_in_shadow)
	{
		for (int j = 0;j<3;j++)
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
