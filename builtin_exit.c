/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:43:11 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/03/24 23:13:23 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(char **args, int last_status)
{
	printf("exit\n");
	if (args[1])
		last_status = ft_atoi(args[1]);
	exit(last_status);
	return (0);
}
