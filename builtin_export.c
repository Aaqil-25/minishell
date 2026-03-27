/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:43:25 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/03/25 22:17:12 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**g_export_only;

static int	key_len(char *s)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != '=')
		len++;
	return (len);
}

static int	valid_identifier(char *s)
{
	int	i;

	if (!s || !s[0] || (!(ft_isalpha(s[0])) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	array_count(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

static int	is_same_key(char *entry, char *arg)
{
	int	len;

	len = key_len(arg);
	return (ft_strncmp(entry, arg, len) == 0 && entry[len] == '=');
}

static char	**clone_with_append(char **src, char *extra)
{
	char	**dst;
	int		i;

	dst = malloc(sizeof(char *) * (array_count(src) + 2));
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

static int	env_set_assignment(char *arg, char ***env)
{
	char	**new_env;
	int		i;

	i = 0;
	while ((*env)[i])
	{
		if (is_same_key((*env)[i], arg))
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(arg);
			return ((*env)[i] != NULL);
		}
		i++;
	}
	new_env = clone_with_append(*env, arg);
	if (!new_env)
		return (0);
	free_array_of_words(env);
	*env = new_env;
	return (1);
}

static int	has_name(char **arr, char *name)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		if (ft_strncmp(arr[i], name, ft_strlen(name) + 1) == 0)
			return (1);
		i++;
	}
	return (0);
}

static void	remove_export_only(char *name)
{
	int	i;
	int	j;
	int	len;

	len = key_len(name);

	i = 0;
	while (g_export_only && g_export_only[i])
	{
		if ((int)ft_strlen(g_export_only[i]) == len
			&& ft_strncmp(g_export_only[i], name, len) == 0)
		{
			free(g_export_only[i]);
			j = i;
			while (g_export_only[j])
			{
				g_export_only[j] = g_export_only[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

static int	add_export_only(char *arg, char **env)
{
	char	*name;
	char	**new_arr;

	name = ft_substr(arg, 0, key_len(arg));
	if (!name)
		return (0);
	if (has_name(g_export_only, name) || exec_get_env_value(env, name))
		return (free(name), 1);
	new_arr = clone_with_append(g_export_only, name);
	free(name);
	if (!new_arr)
		return (0);
	free_array_of_words(&g_export_only);
	g_export_only = new_arr;
	return (1);
}

static char	*decl_from_env(char *entry)
{
	int		len;
	char	*key;
	char	*value;
	char	*tmp;
	char	*out;

	len = key_len(entry);
	key = ft_substr(entry, 0, len);
	value = ft_strdup(entry + len + 1);
	if (!key || !value)
		return (free(key), free(value), NULL);
	tmp = ft_strjoin("declare -x ", key);
	free(key);
	if (!tmp)
		return (free(value), NULL);
	out = ft_strjoin(tmp, "=\"");
	free(tmp);
	if (!out)
		return (free(value), NULL);
	tmp = ft_strjoin(out, value);
	free(out);
	free(value);
	if (!tmp)
		return (NULL);
	out = ft_strjoin(tmp, "\"");
	free(tmp);
	return (out);
}

static void	sort_strs(char **arr)
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
			if (ft_strncmp(arr[i], arr[j], ft_strlen(arr[i])
					+ ft_strlen(arr[j]) + 1) > 0)
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

static void	print_export_all(char **env)
{
	char	**all;
	int		i;
	int		n;

	n = array_count(env) + array_count(g_export_only);
	all = ft_calloc(n + 1, sizeof(char *));
	if (!all)
		return ;
	i = 0;
	while (env && env[i])
	{
		all[i] = decl_from_env(env[i]);
		i++;
	}
	n = 0;
	while (g_export_only && g_export_only[n])
	{
		all[i + n] = ft_strjoin("declare -x ", g_export_only[n]);
		n++;
	}
	sort_strs(all);
	i = 0;
	while (all[i])
		ft_putendl_fd(all[i++], 1);
	free_array_of_words(&all);
}

int	builtin_export(char **args, char ***env)
{
	int	i;
	int	status;

	if (!args[1])
		return (print_export_all(*env), 0);
	i = 1;
	status = 0;
	while (args[i])
	{
		if (!valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
			status = 1;
		}
		else if (ft_strchr(args[i], '='))
		{
			if (!env_set_assignment(args[i], env))
				status = 1;
			remove_export_only(args[i]);
		}
		else if (!add_export_only(args[i], *env))
			status = 1;
		i++;
	}
	return (status);
}
