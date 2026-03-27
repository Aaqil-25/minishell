/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:48:31 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/03/25 22:00:16 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_internal.h"
#include <sys/stat.h>

static char	*join_path(char *dir, char *name)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, name);
	free(tmp);
	return (full);
}

static char	*search_in_path(char **dirs, char *name)
{
	char	*full;
	int		i;

	i = 0;
	while (dirs[i])
	{
		full = join_path(dirs[i], name);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	*exec_find_path(char *name, char **env)
{
	char		*path_env;
	char		**dirs;
	char		*found;
	struct stat	st;

	if (!name || !name[0])
		return (NULL);
	if (ft_strchr(name, '/'))
	{
		if (stat(name, &st) == 0 && access(name, X_OK) == 0)
			return (ft_strdup(name));
		return (NULL);
	}
	path_env = exec_get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	found = search_in_path(dirs, name);
	free_array_of_words(&dirs);
	return (found);
}
