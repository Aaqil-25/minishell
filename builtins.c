/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 00:00:00 by minishell         #+#    #+#             */
/*   Updated: 2025/02/07 00:00:00 by minishell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * export [ NAME=value ... ] with no options.
 * Add or update each arg in env. (Realloc env as needed.)
 */
int	builtin_export(char **args, char ***env)
{
	(void)args;
	(void)env;
	return (0);
}

/*
 * unset [ NAME ... ] with no options.
 * Remove each name from env.
 */
int	builtin_unset(char **args, char ***env)
{
	(void)args;
	(void)env;
	return (0);
}

/*
 * env with no options or arguments. Print env.
 */
int	builtin_env(char **args, char **env)
{
	int	i;

	(void)args;
	i = 0;
	while (env && env[i])
	{
		ft_putendl_fd(env[i], 1);
		i++;
	}
	return (0);
}

/*
 * exit with no options. Exit shell with 0 (or last status).
 */
int	builtin_exit(char **args)
{
	(void)args;
	exit(g_exit_status);
	return (0);
}
