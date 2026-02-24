/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtof.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:57:18 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/20 19:03:56 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	find_newline_eof(char *buffer)
{
	int	i;

	i = 0;
	if (!buffer)
		return (-1);
	while (buffer[i])
	{
		if (buffer[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

/*void	gnl_free_buffer(void)
{
	char	**buffer; 

	buffer = gnl_static_buffer();
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
}*/

long	check_overflow(long *r, int d, int b, int s)
{
	if (s == 1 && *r > (LONG_MAX - d) / b)
		return (LONG_MAX);
	if (s == -1 && (unsigned long)*r > ((unsigned long)LONG_MAX + 1 - d) / b)
		return (LONG_MIN);
	return (0);
}

const char	*fractional_part(float *result, const char *nptr)
{
	int			divisor;
	float		fraction;
	int			has_digits;

	divisor = 1;
	fraction = 0.0f;
	has_digits = 0;
	if (*nptr == '.')
	{
		nptr++;
		while (ft_isdigit(*nptr))
		{
			fraction = fraction * 10.0f + (*nptr - '0');
			divisor *= 10;
			nptr++;
			has_digits = 1;
		}
		if (has_digits)
			*result += fraction / divisor;
		else
			nptr--;
	}
	return (nptr);
}

/// @brief Simple version of strtof, don't handle hexdecimal, sci-notation
/// @brief and special values.
/// @param nptr String to be converted.
/// @param endptr If the function bumps into a non-digital value this
/// variable will store a pointer to that character.
/// @return A converted float-type number
float	ft_strtof(const char *nptr, char **endptr)
{
	float		result;
	int			sign;

	result = 0.0f;
	sign = 1;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-')
	{
		sign = -1;
		nptr++;
	}
	else if (*nptr == '+')
		nptr++;
	while (ft_isdigit(*nptr))
	{
		result = result * 10.0f + (*nptr - '0');
		nptr++;
	}
	nptr = fractional_part(&result, nptr);
	if (endptr)
		*endptr = (char *)nptr;
	return (result * sign);
}
