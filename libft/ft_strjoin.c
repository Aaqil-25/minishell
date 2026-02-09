/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 18:44:01 by ymazzett          #+#    #+#             */
/*   Updated: 2025/01/15 21:11:04 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		l1;
	int		l2;
	int		i;
	char	*output;

	if (!s1 || !s2)
		return (NULL);
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	output = (char *)malloc((l1 + l2 + 1) * sizeof(char));
	if (output == NULL)
		return (NULL);
	i = -1;
	while (++i < l1)
		output[i] = s1[i];
	i = -1;
	while (++i < l2)
		output[l1 + i] = s2[i];
	output[l1 + i] = 0;
	return (output);
}
