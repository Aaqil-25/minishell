/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 00:00:00 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/27 13:24:35 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*export_decl_from_env(char *entry)
{
	int		len;
	char	*key;
	char	*value;
	char	*decl;
	char	*tmp;

	len = export_key_len(entry);
	key = ft_substr(entry, 0, len);
	value = ft_strdup(entry + len + 1);
	if (!key || !value)
		return (free(key), free(value), NULL);
	decl = ft_strjoin("declare -x ", key);
	tmp = ft_strjoin("=\"", value);
	free(key);
	free(value);
	if (!decl || !tmp)
		return (free(decl), free(tmp), NULL);
	value = ft_strjoin(decl, tmp);
	free(decl);
	free(tmp);
	if (!value)
		return (NULL);
	decl = ft_strjoin(value, "\"");
	free(value);
	return (decl);
}

static int	export_should_swap(char *a, char *b)
{
	return (ft_strncmp(a, b, ft_strlen(a) + ft_strlen(b) + 1) > 0);
}

static void	export_sort_strs(char **arr)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (arr && arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (export_should_swap(arr[i], arr[j]))
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static int	export_fill_all(char **all, char **env, char **export_only)
{
	int	i;
	int	n;

	i = 0;
	while (env && env[i])
	{
		all[i] = export_decl_from_env(env[i]);
		i++;
	}
	n = 0;
	while (export_only && export_only[n])
	{
		all[i + n] = ft_strjoin("declare -x ", export_only[n]);
		n++;
	}
	return (i + n);
}

void	print_export_all(char **env)
{
	char	**all;
	char	**export_only;
	int		total;
	int		i;

	export_only = export_only_store(NULL, 0);
	total = export_array_count(env) + export_array_count(export_only);
	all = ft_calloc(total + 1, sizeof(char *));
	if (!all)
		return ;
	export_fill_all(all, env, export_only);
	export_sort_strs(all);
	i = 0;
	while (all[i])
		ft_putendl_fd(all[i++], 1);
	free_array_of_words(&all);
}
