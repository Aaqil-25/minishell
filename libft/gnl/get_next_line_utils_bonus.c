/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:10:08 by ymazzett          #+#    #+#             */
/*   Updated: 2025/03/21 18:10:05 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strndup(char *str, size_t n)
{
	char	*new_str;
	size_t	len;
	size_t	i;

	len = ft_strlen(str);
	if (len < n)
		n = len;
	i = 0;
	new_str = malloc((n + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	while (str[i] && i < n)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

char	*ft_strchr(char *str, int ch)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ch)
			return (str + i);
		i++;
	}
	return (NULL);
}

char	*string_concat(char *s1, char *s2)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	new_str = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	while (s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		new_str[i + j] = s2[j];
		j++;
	}
	new_str[i + j] = '\0';
	return (new_str);
}

void	*double_free(void *p1, void *p2)
{
	void	**ptr1;
	void	**ptr2;

	ptr1 = (void **)p1;
	ptr2 = (void **)p2;
	if (ptr1)
	{
		free(*ptr1);
		*ptr1 = NULL;
	}
	if (ptr2)
	{
		free(*ptr2);
		*ptr2 = NULL;
	}
	return (NULL);
}
