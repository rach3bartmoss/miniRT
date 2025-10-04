/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:01:03 by dopereir          #+#    #+#             */
/*   Updated: 2025/10/04 18:09:09 by dopereir         ###   ########.fr       */
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

float ray_intersection_pl(float *sr_origin, float *sr_dir, t_plane *pl)
{
	float	denom;
	float	plane0_light0[3];
	float	t;

	denom = dot(pl->pl_vector_xyz, sr_dir);
	if (fabsf(denom) < 1e-6f)
		return (-1.0f);

	sub(plane0_light0, pl->pl_xyz, sr_origin); // (P0 - O)
	t = dot(plane0_light0, pl->pl_vector_xyz) / denom;

	if (t > 0.0f)
		return (t);
	else
		return (-1.0f);
}

void	prep_sr_cy_intersect(t_cy_ctx *cy_ctx, float *sr_origin, float *sr_dir)
{
	copy_vectors(cy_ctx->normal, cy_ctx->curr_cy->cy_vector_xyz);
	copy_vectors(cy_ctx->d, sr_dir);
	normalize(cy_ctx->normal, cy_ctx->normal);
	copy_vectors(cy_ctx->origin, sr_origin);
	cy_ctx->half_height = cy_ctx->curr_cy->cy_height / 2.0f;
	cy_ctx->radius = cy_ctx->curr_cy->cy_diameter / 2.0f;
	scale(cy_ctx->hA, cy_ctx->normal, cy_ctx->half_height);
	sub(cy_ctx->base_center, cy_ctx->curr_cy->cy_xyz, cy_ctx->hA);
	add(cy_ctx->top_center, cy_ctx->curr_cy->cy_xyz, cy_ctx->hA);
}

float	comp_finite_height_for_light(float	t_side, t_cy_ctx *cy_ctx)
{
	float	hit_p[3];
	float	scale_td[3];
	float	hitp_minus_center[3];

	scale(scale_td, cy_ctx->d, (float)t_side);
	add(hit_p, cy_ctx->origin, scale_td);
	sub(hitp_minus_center, hit_p, cy_ctx->curr_cy->cy_xyz);
	cy_ctx->y = dot(hitp_minus_center, cy_ctx->normal);
	if (cy_ctx->y >= -cy_ctx->half_height && cy_ctx->y <= cy_ctx->half_height)
	{
		return (cy_ctx->y);//intersection happened
	}
	return (-1.0f);
}

float	ray_intersection_cy(float *sr_origin, float *sr_dir, t_cylinder *cy)
{
	t_cy_ctx	cy_ctx;
	float		t_side;
	double		t_bot_top;

	(void)sr_dir;
	cy_ctx.curr_cy = cy;
	prep_sr_cy_intersect(&cy_ctx, sr_origin, sr_dir);
	calc_v_w(&cy_ctx);
	t_side = solve_t_cylinder(cy_ctx.v, cy_ctx.w, &cy_ctx);
	if (t_side > 0.0)
	{
		t_bot_top = comp_finite_height_for_light(t_side, &cy_ctx);
		if (t_bot_top > 0.0f)
			return (t_bot_top);
	}
	t_bot_top = cylinder_bottom_cap(&cy_ctx, 0);
	if (t_bot_top > 0.0f)
		return (t_bot_top);
	t_bot_top = cylinder_top_cap(&cy_ctx, 0);
	if (t_bot_top > 0.0f)
		return (t_bot_top);
	return (-1.0f);
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

	double	t_sp;
	double	t_pl;
	int		is_in_shadow = 0;
	int		sp_count = set_and_get_occ(-1, SPHERE);
	int	i = 0;
	while (i < sp_count)
	{
		t_sp = ray_intersection_sp_tester(p_offset, light_dir_normalized, scene->sphere[i]);
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
				break;
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
