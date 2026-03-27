/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:42:39 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/03/25 22:17:12 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(char **args)
{
	char	*home;

	if (args[1] && args[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	if (!args[1])
	{
		home = getenv("HOME");
		if (!home)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
		if (chdir(home) != 0)
			return (perror("cd"), 1);
		return (0);
	}
	if (chdir(args[1]) != 0)
		return (perror(args[1]), 1);
	return (0);
}
