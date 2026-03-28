/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:42:59 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:43:01 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **args, char **env)
{
	int	i;

	if (args[1] && ft_strncmp(args[1], "-i", 3) == 0 && !args[2])
		return (0);
	i = 0;
	while (env && env[i])
	{
		ft_putendl_fd(env[i], 1);
		i++;
	}
	return (0);
}
