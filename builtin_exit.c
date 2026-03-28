/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:43:11 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/03/25 22:17:12 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit_requested(int set_value)
{
	static int	requested;

	if (set_value >= 0)
		requested = set_value;
	return (requested);
}

static int	is_valid_exit_arg(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	finish_exit(int status)
{
	last_exit_status(status);
	builtin_exit_requested(1);
	return (status);
}

int	builtin_exit(char **args, int last_status)
{
	int	exit_code;

	builtin_exit_requested(0);
	if (isatty(STDIN_FILENO))
		printf("exit\n");
	if (!args[1])
		return (finish_exit(last_status));
	if (!is_valid_exit_arg(args[1]))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		return (finish_exit(2));
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	exit_code = (unsigned char)ft_atoi(args[1]);
	return (finish_exit(exit_code));
}
