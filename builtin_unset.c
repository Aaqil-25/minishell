/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:48:58 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:48:59 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(char **args, char ***env)
{
	int	i;
	int	ret;

	if (!args || !args[1])
		return (0);
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (env_unset_entry(env, args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
