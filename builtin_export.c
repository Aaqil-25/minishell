/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:43:25 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/03/25 22:17:12 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_identifier(char *s)
{
	int	i;

	if (!s || !s[0] || (!(ft_isalpha(s[0])) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_export(char **args, char ***env)
{
	int	i;
	int	status;

	(void)env;
	i = 1;
	status = 0;
	while (args[i])
	{
		if (!valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
			status = 1;
		}
		i++;
	}
	return (status);
}
