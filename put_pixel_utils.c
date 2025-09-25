/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pixel_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 21:33:01 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/25 13:48:32 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	mrt_put_pixel(t_window *win, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= win->width || y < 0 || y >= win->height)
		return ;

	dst = win->addr + (y * win->line_length + x * (win->bpp / 8));
	*(unsigned int *)dst = color;
}

int	rgb3_to_hex(int rgb[3])
{
	int	r;
	int	g;
	int	b;

	r = rgb[0];
	g = rgb[1];
	b = rgb[2];
	return ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}

int	print_array3(float *target_xyz)
{
	printf("{ %.3f, %.3f, %.3f }\n", target_xyz[0], target_xyz[1], target_xyz[2]);
	return (1);
}
