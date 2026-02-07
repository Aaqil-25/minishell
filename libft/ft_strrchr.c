/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:59:36 by ymazzett          #+#    #+#             */
/*   Updated: 2025/01/14 16:31:44 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ptr;
	char	*res;
	char	ch;

	ptr = (char *)s;
	res = (char *)ptr;
	ch = (char)c;
	while (*ptr)
	{
		if (*ptr == ch)
			res = ptr;
		ptr++;
	}
	if (*ptr == ch)
		res = ptr;
	if (*res != ch)
		return (NULL);
	return (res);
}
