/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hit_record.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:51:24 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/23 23:53:47 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	zero_hit_records(t_hit *records, int count)
{
	t_hit	*ptr;
	t_hit	*end;

	ptr = records;
	end = records + count;
	while (ptr + 4 <= end)
	{
		ptr[0] = (t_hit){0};
		ptr[1] = (t_hit){0};
		ptr[2] = (t_hit){0};
		ptr[3] = (t_hit){0};
		ptr += 4;
	}
	while (ptr < end)
	{
		*ptr = (t_hit){0};
		ptr++;
	}
}

static void	set_default_values(t_hit *records, int count)
{
	t_hit	*ptr;
	t_hit	*end;

	ptr = records;
	end = records + count;
	while (ptr + 4 <= end)
	{
		ptr[0].t = INFINITY;
		ptr[1].t = INFINITY;
		ptr[2].t = INFINITY;
		ptr[3].t = INFINITY;
		ptr[0].object_idx = -1;
		ptr[1].object_idx = -1;
		ptr[2].object_idx = -1;
		ptr[3].object_idx = -1;
		ptr += 4;
	}
	while (ptr < end)
	{
		ptr->t = INFINITY;
		ptr->object_idx = -1;
		ptr++;
	}
}

/// @brief Optimized initialization for hit records array.
/// @param ray_table 
void	init_hit_record(t_ray_table *ray_table)
{
	zero_hit_records(ray_table->hit_record, ray_table->total_rays);
	set_default_values(ray_table->hit_record, ray_table->total_rays);
}
