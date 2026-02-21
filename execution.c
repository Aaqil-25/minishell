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

static int	run_builtin_cmd(t_command *cmd, char ***env, int last_status)
{
	if (exec_apply_redirections(cmd) != 0)
		return (1);
	return (exec_run_builtin(cmd, env, last_status));
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

static int	run_single(t_command *cmd, char ***env, int last_status)
{
	if (!cmd->args || !cmd->args[0])
		return (last_status);
	if (exec_is_builtin(cmd->args[0]))
		return (run_builtin_cmd(cmd, env, last_status));
	return (run_external_cmd(cmd, *env));
}

static int	run_pipeline(t_command *cmds, int n, char ***env, int last_status)
{
	(void)n;
	return (run_single(cmds, env, last_status));
}

int	execute(t_command *cmds, char ***env)
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
