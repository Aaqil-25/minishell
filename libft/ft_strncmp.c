/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 23:47:51 by deck              #+#    #+#             */
/*   Updated: 2025/01/15 22:53:56 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char s1[], const char s2[], size_t n)
{
	size_t				i;
	int					diff;
	unsigned const char	*str1;
	unsigned const char	*str2;

	i = 0;
	diff = 0;
	str1 = (unsigned const char *)s1;
	str2 = (unsigned const char *)s2;
	while (str1[i] == str2[i] && str2[i] && i < n)
		i++;
	if (i != n)
		diff = str1[i] - str2[i];
	return (diff);
}
