/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:46:37 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:46:40 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_internal.h"
#include <sys/wait.h>

static int	run_builtin_cmd(t_command *cmd, char **env, int last_status)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
	{
		if (saved_stdin >= 0)
			close(saved_stdin);
		if (saved_stdout >= 0)
			close(saved_stdout);
		return (perror("dup"), 1);
	}
	status = 1;
	if (exec_apply_redirections(cmd) == 0)
		status = exec_run_builtin(cmd, &env, last_status);
	if (dup2(saved_stdin, STDIN_FILENO) < 0)
		perror("dup2");
	if (dup2(saved_stdout, STDOUT_FILENO) < 0)
		perror("dup2");
	close(saved_stdin);
	close(saved_stdout);
	return (status);
}

static int	run_external_cmd(t_command *cmd, char **env)
{
	pid_t	pid;
	char	*path;
	int		status;

	path = exec_find_path(cmd->args[0], env);
	if (!path)
		return (ft_putstr_fd("minishell: command not found\n", 2), 127);
	pid = fork();
	if (pid == 0)
	{
		if (exec_apply_redirections(cmd) != 0)
			exit(1);
		execve(path, cmd->args, env);
		perror(cmd->args[0]);
		exit(127);
	}
	free(path);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	run_single(t_command *cmd, char **env, int last_status)
{
	if (!cmd->args || !cmd->args[0])
		return (last_status);
	if (exec_is_builtin(cmd->args[0]))
		return (run_builtin_cmd(cmd, env, last_status));
	return (run_external_cmd(cmd, env));
}

static int	run_pipeline(t_command *cmds, int n, char **env, int last_status)
{
	int			i;
	int			prev_pipe[2];
	pid_t		*pids;
	t_command	*cmd;
	int			current_pipe[2];

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
		return (perror("malloc"), 1);
	cmd = cmds;
	i = 0;
	while (i < n)
	{
		if (i < n - 1)
		{
			if (pipe(current_pipe) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pids[i] == 0)
		{
			if (i > 0)
			{
				dup2(prev_pipe[0], STDIN_FILENO);
				close(prev_pipe[0]);
				close(prev_pipe[1]);
			}
			if (i < n - 1)
			{
				close(current_pipe[0]);
				dup2(current_pipe[1], STDOUT_FILENO);
				close(current_pipe[1]);
			}
			run_single(cmd, env, last_status);
			exit(EXIT_FAILURE);
		}
		if (i > 0)
		{
			close(prev_pipe[0]);
			close(prev_pipe[1]);
		}
		if (i < n - 1)
		{
			prev_pipe[0] = current_pipe[0];
			prev_pipe[1] = current_pipe[1];
		}
		i++;
		cmd = cmd->next;
	}
	if (n > 1)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	i = 0;
	while (i < n)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
	free(pids);
	return (0);
}

int	execute(t_command *cmds, char **env)
{
	t_command	*cur;
	int			n;

	if (!cmds || !cmds->args || !cmds->args[0])
		return (0);
	cur = cmds;
	n = 0;
	while (cur)
	{
		n++;
		cur = cur->next;
	}
	if (n == 1)
		return (run_single(cmds, env, 0));
	return (run_pipeline(cmds, n, env, 0));
}
