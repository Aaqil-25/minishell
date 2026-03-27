/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 00:00:00 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/27 13:24:35 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**export_only_store(char **new_value, int set)
{
	static char	**export_only;

	if (set)
		export_only = new_value;
	return (export_only);
}

int	export_key_len(char *s)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != '=')
		len++;
	return (len);
}

int	export_array_count(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

int	export_is_same_key(char *entry, char *arg)
{
	int	len;

	len = export_key_len(arg);
	return (ft_strncmp(entry, arg, len) == 0 && entry[len] == '=');
}

char	**export_clone_with_append(char **src, char *extra)
{
	char	**dst;
	int		i;

	dst = malloc(sizeof(char *) * (export_array_count(src) + 2));
	if (!dst)
		return (NULL);
	i = 0;
	while (src && src[i])
	{
		dst[i] = ft_strdup(src[i]);
		if (!dst[i])
			return (free_array_of_words(&dst), NULL);
		i++;
	}
	dst[i] = ft_strdup(extra);
	if (!dst[i])
		return (free_array_of_words(&dst), NULL);
	dst[i + 1] = NULL;
	return (dst);
}
