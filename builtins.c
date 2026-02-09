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
#include <limits.h>

/*
 * echo [ -n ] [ arg ... ]
 * Print args; if -n, do not output trailing newline.
 */
int	builtin_echo(char **args)
{
	int	i;
	int	nl;

	nl = 1;
	i = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 3) == 0)
	{
		nl = 0;
		i = 2;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (nl)
		write(1, "\n", 1);
	return (0);
}

/*
 * cd [ dir ]
 * Only relative or absolute path. chdir(args[1]); on failure perror and 1.
 */
int	builtin_cd(char **args)
{
	if (!args[1])
		return (0);
	if (chdir(args[1]) != 0)
		return (perror(args[1]), 1);
	return (0);
}

/*
 * pwd with no options.
 */
int	builtin_pwd(char **args)
{
	char	buf[PATH_MAX];

	(void)args;
	if (getcwd(buf, sizeof(buf)) == NULL)
		return (perror("pwd"), 1);
	ft_putendl_fd(buf, 1);
	return (0);
}

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
