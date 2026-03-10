/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 00:21:55 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/10 00:33:42 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_internal.h"

pid_t	*allocate_pids(int n)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
		perror("malloc");
	return (pids);
}

void	close_pipe_ends(int fd0, int fd1)
{
	if (fd0 != -1)
		close(fd0);
	if (fd1 != -1)
		close(fd1);
}

void	setup_child_redirections(int i, int n, int *prev, int *curr)
{
	if (i > 0)
	{
		dup2(prev[0], STDIN_FILENO);
		close_pipe_ends(prev[0], prev[1]);
	}
	if (i < n - 1)
	{
		close(curr[0]);
		dup2(curr[1], STDOUT_FILENO);
		close(curr[1]);
	}
}

void	update_prev_pipe(int i, int n, int *prev, int *curr)
{
	if (i > 0)
		close_pipe_ends(prev[0], prev[1]);
	if (i < n - 1)
	{
		prev[0] = curr[0];
		prev[1] = curr[1];
	}
}

void	wait_for_all(pid_t *pids, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}
