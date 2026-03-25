/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:46:37 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/03/25 22:00:16 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_internal.h"
#include <sys/wait.h>
#include <sys/stat.h>

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
	// printf("Builtin command exited with status: %d\n", status);
	last_exit_status(status);
	return (status);
}

static void external_exit_status(int status)
{
	if (WIFEXITED(status))
	{
		// printf("External command exited with status: %d\n", WEXITSTATUS(status));
		last_exit_status(WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status))
	{
		// printf("External command terminated by signal: %d\n", WTERMSIG(status));
		last_exit_status(128 + WTERMSIG(status));
	}
	// else
	// 	printf("External command terminated with unknown status\n");
}

static int	run_external_cmd(t_command *cmd, char **env)
{
	pid_t	pid;
	char	*path;
	int		status;
	struct stat	st;

	path = exec_find_path(cmd->args[0], env);
	if (!path)
	{
		if (ft_strchr(cmd->args[0], '/') && stat(cmd->args[0], &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
				ft_putstr_fd("minishell: Is a directory\\n", 2);
			else
				ft_putstr_fd("minishell: Permission denied\\n", 2);
			return (126);
		}
		ft_putstr_fd("minishell: command not found\\n", 2);
		return (127);
	}
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
	external_exit_status(status);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	run_single(t_command *cmd, char **env, int last_status)
{
	if (!cmd->args || !cmd->args[0])
		return (last_status);
	if (exec_is_builtin(cmd->args[0]))
		return (run_builtin_cmd(cmd, env, last_status));
	return (run_external_cmd(cmd, env));
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
