/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 15:10:00 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/26 12:52:02 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_dup(char **envp)
{
	char	**new_env;
	int		i;
	int		count;

	if (!envp)
		return (NULL);
	count = 0;
	while (envp[count])
		count++;
	new_env = ft_calloc(count + 1, sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			free_array_of_words(&new_env);
			return (NULL);
		}
		i++;
	}
	return (new_env);
}

int	env_find_index(char **env, const char *name)
{
	size_t	len;
	int		i;

	if (!env || !name)
		return (-1);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char	**env_copy_without_index(char **env, int idx, size_t count)
{
	int		i;
	char	**new_env;

	new_env = ft_calloc(count, sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (i < idx)
			new_env[i] = env[i];
		else if (i > idx)
			new_env[i - 1] = env[i];
		i++;
	}
	return (new_env);
}

int	env_unset_entry(char ***env, const char *name)
{
	int		idx;
	size_t	count;
	char	**new_env;

	if (!env || !*env || !name)
		return (-1);
	idx = env_find_index(*env, name);
	if (idx == -1)
		return (0);
	count = arraylen(*env);
	if (count == 0)
		return (-1);
	new_env = env_copy_without_index(*env, idx, count);
	if (!new_env)
		return (-1);
	free((*env)[idx]);
	free(*env);
	*env = new_env;
	return (0);
}
