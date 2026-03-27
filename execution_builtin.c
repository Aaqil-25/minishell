/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:46:37 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/03/25 22:00:16 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_internal.h"
#include <sys/wait.h>

static int	save_std_streams(int *in, int *out)
{
	*in = dup(STDIN_FILENO);
	*out = dup(STDOUT_FILENO);
	if (*in < 0 || *out < 0)
	{
		if (*in >= 0)
			close(*in);
		if (*out >= 0)
			close(*out);
		return (perror("dup"), -1);
	}
	return (0);
}

static void	restore_std_streams(int in, int out)
{
	if (dup2(in, STDIN_FILENO) < 0)
		perror("dup2");
	if (dup2(out, STDOUT_FILENO) < 0)
		perror("dup2");
	close(in);
	close(out);
}

int	run_builtin_cmd(t_command *cmd, char ***env, int last_status)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (save_std_streams(&saved_stdin, &saved_stdout) < 0)
		return (1);
	status = 1;
	if (exec_apply_redirections(cmd) == 0)
		status = exec_run_builtin(cmd, env, last_status);
	restore_std_streams(saved_stdin, saved_stdout);
	last_exit_status(status);
	return (status);
}

void	external_exit_status(int status)
{
	if (WIFEXITED(status))
		last_exit_status(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		last_exit_status(128 + WTERMSIG(status));
}
