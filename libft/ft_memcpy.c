/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:38:52 by ymazzett          #+#    #+#             */
/*   Updated: 2025/01/08 17:43:08 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*s1;
	char	*s2;
	int		i;

	s1 = (char *)dst;
	s2 = (char *)src;
	i = 0;
	while (n > 0)
	{
		s1[i] = s2[i];
		n--;
		i++;
	}
	return (s1);
}
