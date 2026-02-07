/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:03:38 by ymazzett          #+#    #+#             */
/*   Updated: 2025/01/15 23:45:19 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*fillit(const char *s, unsigned int start, size_t slen, size_t len)
{
	size_t	i;
	size_t	j;
	char	*output;

	i = 0;
	j = 0;
	output = (char *)malloc(sizeof(*s) * (len + 1));
	if (!output)
		return (NULL);
	while (i < slen && j < len)
	{
		if (i >= start && j < len)
		{
			output[j] = s[i];
			j++;
		}
		i++;
	}
	output[j] = 0;
	return (output);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*output;
	size_t		slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (len > ft_strlen(s))
		len = ft_strlen(s);
	output = fillit(s, start, slen, len);
	return (output);
}
