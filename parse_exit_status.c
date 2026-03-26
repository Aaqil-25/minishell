/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exit_status.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:01:43 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/25 21:02:33 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	last_exit_status(int new_status)
{
	static int	status = 0;

	if (new_status != -1)
		status = new_status;
	return (status);
}

int	is_redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == APPEND || type == HEREDOC);
}
