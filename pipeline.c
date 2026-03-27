/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 23:22:38 by ymazzett          #+#    #+#             */
/*   Updated: 2026/03/25 20:35:09 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_internal.h"

void	create_next_pipe(int i, int n, int *pipefd)
{
	if (i < n - 1 && pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

pid_t	fork_child(t_child_process_params params, char **env, int last)
{
	pid_t	pid;
	char	**env_ref;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		env_ref = env;
		setup_child_redirections(params.i, params.n, params.prev, params.curr);
		exit(run_single(params.cmd, &env_ref, last));
	}
	return (pid);
}

static void	init_pipeline(int *prev)
{
	prev[0] = -1;
	prev[1] = -1;
}

int	run_pipeline(t_command *cmds, int n, char **env, int last_status)
{
	t_child_process_params	params;
	pid_t					*pids;
	int						status;

	params.n = n;
	init_pipeline(params.prev);
	pids = allocate_pids(params.n);
	if (!pids)
		return (1);
	params.cmd = cmds;
	params.i = 0;
	while (params.i < params.n)
	{
		create_next_pipe(params.i, params.n, params.curr);
		pids[params.i] = fork_child(params, env, last_status);
		update_prev_pipe(params.i, params.n, params.prev, params.curr);
		params.i++;
		params.cmd = params.cmd->next;
	}
	if (n > 1)
		close_pipe_ends(params.prev[0], params.prev[1]);
	status = wait_for_all(pids, params.n);
	free(pids);
	last_exit_status(status);
	return (status);
}
