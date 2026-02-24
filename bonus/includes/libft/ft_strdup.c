/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:25:36 by dopereir          #+#    #+#             */
/*   Updated: 2026/02/19 01:00:45 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*duplicate;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	duplicate = (char *)malloc((len + 1) * sizeof(char));
	if (duplicate != NULL)
		ft_strlcpy(duplicate, s, len + 1);
	return (duplicate);
}
