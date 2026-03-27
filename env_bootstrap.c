/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bootstrap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 00:00:00 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/27 16:06:01 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_if_missing(char ***env, char *name, char *entry)
{
	char	**new_env;

	if (exec_get_env_value(*env, name))
		return ;
	new_env = append_to_array(*env, entry);
	if (new_env)
		*env = new_env;
}

void	init_default_env(char ***env)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	if (getcwd(cwd, sizeof(cwd)))
	{
		pwd = ft_strjoin("PWD=", cwd);
		if (pwd)
		{
			add_if_missing(env, "PWD", pwd);
			free(pwd);
		}
	}
	add_if_missing(env, "LS_COLORS", "LS_COLORS=");
	add_if_missing(env, "LESSCLOSE", "LESSCLOSE=/usr/bin/lesspipe %s %s");
	add_if_missing(env, "LESSOPEN", "LESSOPEN=| /usr/bin/lesspipe %s");
	add_if_missing(env, "_", "_=/usr/bin/env");
}
