/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:42:13 by ymazzett          #+#    #+#             */
/*   Updated: 2025/01/12 04:18:52 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	const size_t	n = count * size;
	char			*output;

	if (!n)
		return (malloc(0));
	if (size > INT_MAX / count)
		return (NULL);
	output = malloc(n);
	if (output)
		ft_bzero(output, n);
	return (output);
}
