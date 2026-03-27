/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_ext.c                                    :+:      :+:    :+:   */
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
#include <errno.h>

static void	report_execve_error(char *name)
{
	struct stat	st;

	if (stat(name, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
	if (errno == EACCES)
	{
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	perror(name);
	if (errno == ENOENT)
		exit(127);
	exit(126);
}

static void	try_execve_child(t_command *cmd, char **env, char *path)
{
	if (exec_apply_redirections(cmd, env) != 0)
		exit(1);
	execve(path, cmd->args, env);
	report_execve_error(cmd->args[0]);
}

static int	report_path_errors(t_command *cmd)
{
	struct stat	st;
	char		*tmp;

	if (ft_strchr(cmd->args[0], '/') && stat(cmd->args[0], &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			tmp = ft_strjoin(cmd->args[0], ": Is a directory\n");
		else
			tmp = ft_strjoin(cmd->args[0], ": Permission denied\n");
		ft_putstr_fd(tmp, 2);
		free(tmp);
		return (126);
	}
	tmp = ft_strjoin(cmd->args[0], ": command not found\n");
	ft_putstr_fd(tmp, 2);
	free(tmp);
	return (127);
}

int	run_external_cmd(t_command *cmd, char **env)
{
	pid_t	pid;
	char	*path;
	int		status;
	pid_t	wpid;

	path = exec_find_path(cmd->args[0], env);
	if (!path)
		return (report_path_errors(cmd));
	pid = fork();
	if (pid == 0)
		try_execve_child(cmd, env, path);
	free(path);
	wpid = waitpid(pid, &status, 0);
	while (wpid < 0 && errno == EINTR)
		wpid = waitpid(pid, &status, 0);
	if (wpid < 0)
		return (perror("waitpid"), 1);
	external_exit_status(status);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
