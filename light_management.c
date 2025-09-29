/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:01:03 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/29 22:08:28 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	normalize_colors(float rgb[3])
{
	rgb[0] /= 255.0f;
	rgb[1] /= 255.0f;
	rgb[2] /= 255.0f;
}

//Color = a * (ca,change / 255) * (co,change / 255)
void	apply_ambient_light(t_scene *scene, t_hit *curr_rec, int a_shade[3])
{
	float	a;
	float	ca[3];
	float	co[3];
	float	term;

	a = scene->ambiance->light_ratio;
	ca[0] = (float)scene->ambiance->a_rgb[0];
	ca[1] = (float)scene->ambiance->a_rgb[1];
	ca[2] = (float)scene->ambiance->a_rgb[2];

	co[0] = (float)curr_rec->color[0];
	co[1] = (float)curr_rec->color[1];
	co[2] = (float)curr_rec->color[2];
	normalize_colors(ca);
	normalize_colors(co);

	for (int i = 0; i < 3; ++i)
	{
		term = a * ca[i] * co[i];			// ambient = a × Ca × Co
		term = fmaxf(term, 0.0f);			// clamp bottom
		term = fminf(term, 1.0f);			// clamp top
		a_shade[i] = (int)(term * 255.0f);	// back to [0–255]
	}

	//dont overwrite the original color
}
