/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 19:29:34 by ymazzett          #+#    #+#             */
/*   Updated: 2025/01/14 16:31:58 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isinside(const char *str, char c)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == c)
			return (1);
	return (0);
}

static const char	*ft_tofindfirst(char const *s1, char const *set)
{
	int			i;
	const char	*ptr;

	i = 0;
	while (s1[i] && isinside(set, s1[i]))
		i++;
	ptr = s1 + i;
	return (ptr);
}

static const char	*ft_tofindlast(const char *ptr, const char *set)
{
	const char	*pread;
	int			i;

	pread = ptr;
	while (*pread)
	{
		i = 0;
		while (pread[i] && isinside(set, pread[i]))
		{
			i++;
		}
		if (pread[i] == 0)
			return (pread);
		pread++;
	}
	return (pread);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int			i;
	const char	*ptr;
	const char	*ptr2;
	char		*output;

	ptr = ft_tofindfirst(s1, set);
	ptr2 = ft_tofindlast(ptr, set);
	i = 0;
	while (&ptr[i] != ptr2)
		i++;
	output = (char *)malloc((i + 1) * sizeof(char));
	if (output == NULL)
		return (output);
	i = 0;
	while (&ptr[i] != ptr2)
	{
		output[i] = ptr[i];
		i++;
	}
	output[i] = 0;
	return (output);
}
