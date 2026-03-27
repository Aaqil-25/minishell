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

static int	valid_identifier(char *s)
{
	int	i;

	if (!s || !s[0] || (!(ft_isalpha(s[0])) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
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

	i = 1;
	while (args[i])
	{
		if (!valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: not a valid identifier\n", 2);
		}
		else
		{
			remove_export_only(args[i]);
			remove_from_env(args[i], env);
		}
		i++;
	}
	return (0);
}
