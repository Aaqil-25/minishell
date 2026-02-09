/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:48:24 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:48:26 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_internal.h"
#include <fcntl.h>

static int	open_and_dup(char *file, int flags, int target)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd < 0)
		return (perror(file), -1);
	dup2(fd, target);
	close(fd);
	return (0);
}

int	exec_apply_redirections(t_command *cmd)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == C_REDIR_IN
			&& open_and_dup(r->filename, O_RDONLY, STDIN_FILENO) != 0)
			return (-1);
		if (r->type == C_REDIR_OUT
			&& open_and_dup(r->filename, O_WRONLY | O_CREAT | O_TRUNC,
				STDOUT_FILENO) != 0)
			return (-1);
		if (r->type == C_REDIR_APPEND
			&& open_and_dup(r->filename, O_WRONLY | O_CREAT | O_APPEND,
				STDOUT_FILENO) != 0)
			return (-1);
		r = r->next;
	}
	return (0);
}
