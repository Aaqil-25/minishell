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

static int	unset_invalid_option(char *arg)
{
	ft_putstr_fd("minishell: unset: ", 2);
	write(2, arg, 2);
	ft_putstr_fd(": invalid option\n", 2);
	ft_putstr_fd("unset: usage: unset [-f] [-v] [-n] [name ...]\n", 2);
	return (2);
}

static int	unset_parse_options(char **args, int *start)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] != '\0')
	{
		if (ft_strncmp(args[i], "--", 3) == 0)
		{
			i++;
			break ;
		}
		j = 1;
		while (args[i][j])
		{
			if (args[i][j] != 'f' && args[i][j] != 'v' && args[i][j] != 'n')
				return (unset_invalid_option(args[i]));
			j++;
		}
		i++;
	}
	*start = i;
	return (0);
}

static int	is_same_key(char *entry, char *var_name)
{
	int	len;

	len = ft_strlen(var_name);
	return (ft_strncmp(entry, var_name, len) == 0 && entry[len] == '=');
}

static int	remove_from_env(char *var_name, char ***env)
{
	int	i;
	int	j;

	i = 0;
	while ((*env)[i])
	{
		if (is_same_key((*env)[i], var_name))
		{
			free((*env)[i]);
			j = i;
			while ((*env)[j])
			{
				(*env)[j] = (*env)[j + 1];
				j++;
			}
			return (1);
		}
		i++;
	}
	return (0);
}

int	builtin_unset(char **args, char ***env)
{
	int	i;
	int	status;

	status = unset_parse_options(args, &i);
	if (status)
		return (status);
	while (args[i])
	{
		remove_export_only(args[i]);
		remove_from_env(args[i], env);
		i++;
	}
	return (0);
}
