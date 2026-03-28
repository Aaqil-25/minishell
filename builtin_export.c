/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:43:25 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/03/28 11:35:53 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_identifier(char *s)
{
	int	i;

	if (!s || !s[0] || (!(ft_isalpha(s[0])) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=' && s[i] != '+')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	if (s[i] == '+' && s[i + 1] != '=')
		return (0);
	return (1);
}

static int	export_parse_option(char *arg, char ***env, int *status)
{
	if (ft_strncmp(arg, "-p", 3) == 0)
	{
		print_export_all(*env);
		return (1);
	}
	if (arg[0] == '-' && arg[1] != '\0')
	{
		ft_putstr_fd("minishell: export: ", 2);
		if (arg[1] == '-')
			ft_putstr_fd("--", 2);
		else
			write(2, arg, 2);
		ft_putstr_fd(": invalid option\n", 2);
		*status = 2;
		return (1);
	}
	return (0);
}

static int	export_set_arg(char *arg, char ***env)
{
	char	*entry;

	entry = arg;
	if (ft_strchr(arg, '+') && ft_strchr(arg, '='))
	{
		entry = export_build_append_arg(arg, *env);
		if (!entry)
			return (0);
	}
	if (!export_set_assignment(entry, env))
	{
		if (entry != arg)
			free(entry);
		return (0);
	}
	if (entry != arg)
		free(entry);
	remove_export_only(arg);
	return (1);
}

static void	export_process_arg(char *arg, char ***env, int *status)
{
	if (arg[0] == '\\' && (ft_isalpha(arg[1]) || arg[1] == '_'))
		arg++;
	if (!valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
		*status = 1;
	}
	else if (ft_strchr(arg, '='))
	{
		if (!export_set_arg(arg, env))
			*status = 1;
	}
	else if (!add_export_only(arg, *env))
		*status = 1;
}

int	builtin_export(char **args, char ***env)
{
	int	i;
	int	status;
	int	parse_options;

	if (!args[1])
		return (print_export_all(*env), 0);
	i = 1;
	status = 0;
	parse_options = 1;
	while (args[i])
	{
		if (parse_options && export_parse_option(args[i], env, &status))
		{
			if (status == 2)
				break ;
			i++;
			continue ;
		}
		parse_options = 0;
		export_process_arg(args[i], env, &status);
		i++;
	}
	return (status);
}
