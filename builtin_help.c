/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 00:00:00 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/28 00:00:00 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <sys/wait.h>

static int	wait_child_status(pid_t pid)
{
	int		status;
	pid_t	wpid;

	wpid = waitpid(pid, &status, 0);
	while (wpid < 0 && errno == EINTR)
		wpid = waitpid(pid, &status, 0);
	if (wpid < 0)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	builtin_help(char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		execve("/bin/bash", (char *[]){"bash", "-c", "help", NULL}, env);
		perror("help");
		exit(127);
	}
	return (wait_child_status(pid));
}
