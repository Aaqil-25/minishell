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

static char	*normalize_export_arg(char *arg)
{
	if (arg && arg[0] == '\\' && (ft_isalpha(arg[1]) || arg[1] == '_'))
		return (arg + 1);
	return (arg);
}

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

static void	export_process_arg(char *arg, char ***env, int *status)
{
	arg = normalize_export_arg(arg);
	if (!valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
		*status = 1;
	}
	else if (ft_strchr(arg, '='))
	{
		if (!export_set_assignment(arg, env))
			*status = 1;
		remove_export_only(arg);
	}
	else if (!add_export_only(arg, *env))
		*status = 1;
}

int	builtin_export(char **args, char ***env)
{
	int	i;
	int	status;

	if (!args[1])
		return (print_export_all(*env), 0);
	i = 1;
	status = 0;
	while (args[i])
	{
		export_process_arg(args[i], env, &status);
		i++;
	}
	return (status);
}
