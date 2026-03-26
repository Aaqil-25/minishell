/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:21:35 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/10 17:26:49 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
