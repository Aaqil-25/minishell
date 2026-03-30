/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_mutate.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 00:00:00 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/28 11:49:47 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_has_name(char **arr, char *name)
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

void	remove_export_only(char *name)
{
	char	**export_only;
	int		i;
	int		j;
	int		len;

	export_only = export_only_store(NULL, 0);
	len = export_key_len(name);
	i = 0;
	while (export_only && export_only[i])
	{
		if ((int)ft_strlen(export_only[i]) == len
			&& ft_strncmp(export_only[i], name, len) == 0)
		{
			free(export_only[i]);
			j = i;
			while (export_only[j])
			{
				export_only[j] = export_only[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

int	export_set_assignment(char *arg, char ***env)
{
	char	**new_env;
	int		i;

	i = 0;
	while ((*env)[i])
	{
		if (export_is_same_key((*env)[i], arg))
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(arg);
			return ((*env)[i] != NULL);
		}
		i++;
	}
	new_env = export_clone_with_append(*env, arg);
	if (!new_env)
		return (0);
	free_array_of_words(env);
	*env = new_env;
	return (1);
}

int	add_export_only(char *arg, char **env)
{
	char	*name;
	char	**export_only;
	char	**new_arr;

	export_only = export_only_store(NULL, 0);
	name = ft_substr(arg, 0, export_key_len(arg));
	if (!name)
		return (0);
	if (export_has_name(export_only, name) || exec_get_env_value(env, name))
		return (free(name), 1);
	new_arr = export_clone_with_append(export_only, name);
	free(name);
	if (!new_arr)
		return (0);
	free_array_of_words(&export_only);
	export_only_store(new_arr, 1);
	return (1);
}

char	*export_build_append_arg(char *arg, char **env)
{
	char	*plus_eq;
	char	*name;
	char	*value;
	char	*result;

	plus_eq = ft_strchr(arg, '+');
	if (!plus_eq || plus_eq[1] != '=')
		return (ft_strdup(arg));
	name = ft_substr(arg, 0, plus_eq - arg);
	if (!name)
		return (NULL);
	value = exec_get_env_value(env, name);
	if (value)
		result = ft_strjoin(value, plus_eq + 2);
	else
		result = ft_strdup(plus_eq + 2);
	if (!result)
		return (free(name), NULL);
	value = ft_strjoin(name, "=");
	if (!value)
		return (free(name), free(result), NULL);
	free(name);
	name = ft_strjoin(value, result);
	return (free(value), free(result), name);
}
