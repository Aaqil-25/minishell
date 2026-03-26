/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:43:39 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:43:44 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

int	builtin_pwd(char **args)
{
	char	buf[PATH_MAX];

	(void)args;
	if (getcwd(buf, sizeof(buf)) == NULL)
		return (perror("pwd"), 1);
	ft_putendl_fd(buf, 1);
	return (0);
}
