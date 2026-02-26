/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:43:25 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/26 13:35:58 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_existing(char *arg, char ***env, int idx)
{
	char	*dup;

	dup = ft_strdup(arg);
	if (!dup)
		return (1);
	free((*env)[idx]);
	(*env)[idx] = dup;
	return (0);
}

static int	append_new(char *arg, char ***env)
{
	size_t	count;
	char	**new_env;
	int		i;

	count = arraylen(*env);
	new_env = ft_calloc(count + 2, sizeof(char *));
	if (!new_env)
		return (1);
	i = 0;
	while (i < (int)count)
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[count] = ft_strdup(arg);
	if (!new_env[count])
		return (free(new_env), 1);
	free(*env);
	*env = new_env;
	return (0);
}

static int	export_one(char *arg, char ***env)
{
	int	idx;

	if (!arg || !ft_strchr(arg, '='))
		return (0);
	idx = env_find_index(*env, arg);
	if (idx >= 0)
		return (update_existing(arg, env, idx));
	return (append_new(arg, env));
}

int	builtin_export(char **args, char ***env)
{
	int	i;
	int	ret;

	if (!args || !args[1])
		return (0);
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (export_one(args[i], env) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
