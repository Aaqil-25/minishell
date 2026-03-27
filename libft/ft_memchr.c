/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 22:52:13 by ymazzett          #+#    #+#             */
/*   Updated: 2025/01/15 22:53:30 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char			*ptr;
	unsigned char			ch;
	size_t					i;

	ptr = (unsigned char *)s;
	ch = (unsigned char)c;
	i = 0;
	while (i < n && *ptr != ch)
	{
		ptr++;
		i++;
	}
	if (*ptr == ch && n != 0 && i != n)
		return (ptr);
	return (NULL);
}
