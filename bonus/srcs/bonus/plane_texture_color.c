/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_texture_color.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-vri <joao-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 01:58:13 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/23 21:24:59 by joao-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	apply_plane_texture(t_hit *hit, t_texture *tex, t_plane *pl)
{
	t_pl_tex_ctx	ctx;
	double			helper[3];

	set_vec_double_values(helper, 0, 1, 0);
	memset(&ctx, 0, sizeof(t_pl_tex_ctx));
	if (fabs(pl->pl_vector_xyz[1]) < 0.999f)
		cross(pl->pl_vector_xyz, helper, ctx.tan);
	else
	{
		set_vec_double_values(helper, 1, 0, 0);
		cross(pl->pl_vector_xyz, helper, ctx.tan);
	}
	normalize(ctx.tan, ctx.tan);
	cross(ctx.tan, pl->pl_vector_xyz, ctx.bi_tan);
	normalize(ctx.bi_tan, ctx.bi_tan);
	sub(ctx.vec, hit->hit_point, pl->pl_xyz);
	ctx.uv[0] = dot(ctx.vec, ctx.tan) * TEXTURE_SCALE;
	ctx.uv[1] = dot(ctx.vec, ctx.bi_tan) * TEXTURE_SCALE;
	ctx.uv[0] = ctx.uv[0] - floorf(ctx.uv[0]);
	ctx.uv[1] = ctx.uv[1] - floorf(ctx.uv[1]);
	ctx.xy[0] = (int)(ctx.uv[0] * tex->width);
	ctx.xy[1] = (int)(ctx.uv[1] * tex->height);
	get_texture_color(tex, ctx.xy[0], ctx.xy[1], hit->color);
}
