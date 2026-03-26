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

static void	try_execve_child(t_command *cmd, char **env, char *path)
{
	if (exec_apply_redirections(cmd) != 0)
		exit(1);
	execve(path, cmd->args, env);
	perror(cmd->args[0]);
	exit(127);
}

static int	report_path_errors(t_command *cmd)
{
	struct stat	st;

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

int	run_external_cmd(t_command *cmd, char **env)
{
	pid_t	pid;
	char	*path;
	int		status;

	path = exec_find_path(cmd->args[0], env);
	if (!path)
		return (report_path_errors(cmd));
	pid = fork();
	if (pid == 0)
		try_execve_child(cmd, env, path);
	free(path);
	waitpid(pid, &status, 0);
	external_exit_status(status);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
