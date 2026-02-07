/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:38:55 by ymazzett          #+#    #+#             */
/*   Updated: 2025/01/14 16:30:11 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*dest;
	const char	*source;

	dest = dst;
	source = src;
	if (source < dest && dest < source + len)
	{
		dest += len;
		source += len;
		while (0 < len)
		{
			*--dest = *--source;
			--len;
		}
	}
	else
	{
		while (0 < len)
		{
			*dest++ = *source++;
			--len;
		}
	}
	return (dst);
}
