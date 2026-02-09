/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 00:00:00 by minishell         #+#    #+#             */
/*   Updated: 2025/02/07 00:00:00 by minishell        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

volatile sig_atomic_t	g_signal_number = 0;
int						g_exit_status = 0;

/*
 * Apply all redirections for this command. Returns 0 on success, -1 on error.
 * For HEREDOC, read from stdin until line equals delimiter; use a temp file
 * or pipe and redirect stdin from it.
 */
static int	apply_redirections(t_command *cmd)
{
	t_redir	*r;
	int		fd;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == C_REDIR_IN)
		{
			fd = open(r->filename, O_RDONLY);
			if (fd < 0)
				return (perror(r->filename), -1);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (r->type == C_REDIR_OUT)
		{
			fd = open(r->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				return (perror(r->filename), -1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (r->type == C_REDIR_APPEND)
		{
			fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				return (perror(r->filename), -1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (r->type == C_REDIR_HEREDOC)
		{
			/* TODO: read stdin until line == r->filename, write to temp or pipe,
			   then open/dup2 that as stdin. See EXECUTION_GUIDE.md Step 5. */
			(void)r;
		}
		r = r->next;
	}
	return (0);
}

/*
 * Get value of env var NAME from env array (entries "NAME=value"). Returns
 * pointer into env string or NULL. Do not free.
 */
static char	*get_env_value(char **env, char *name)
{
	size_t	len;
	int		i;

	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (&env[i][len + 1]);
		i++;
	}
	return (NULL);
}

/*
 * Return 1 if args[0] is a builtin name, 0 otherwise.
 */
static int	is_builtin(char *name)
{
	if (!name)
		return (0);
	if (ft_strncmp(name, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(name, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(name, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(name, "export", 7) == 0)
		return (1);
	if (ft_strncmp(name, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(name, "env", 4) == 0)
		return (1);
	if (ft_strncmp(name, "exit", 5) == 0)
		return (1);
	return (0);
}

/*
 * Run builtin by name. Only call when is_builtin(args[0]). Returns exit status.
 */
static int	run_builtin(t_command *cmd, char ***env)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (builtin_echo(cmd->args));
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (builtin_cd(cmd->args));
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (builtin_pwd(cmd->args));
	if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (builtin_export(cmd->args, env));
	if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (builtin_unset(cmd->args, env));
	if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (builtin_env(cmd->args, *env));
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (builtin_exit(cmd->args));
	return (0);
}

/*
 * Find executable path: if args[0] contains '/', use it; else search PATH.
 * Return allocated path or NULL. Caller must free.
 */
static char	*find_path(char *name, char **env)
{
	char	*path_env;
	char	**dirs;
	char	*slash;
	char	*full;
	int		i;

	if (!name || !name[0])
		return (NULL);
	if (ft_strchr(name, '/'))
	{
		if (access(name, X_OK) == 0)
			return (ft_strdup(name));
		return (NULL);
	}
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		slash = ft_strjoin(dirs[i], "/");
		if (!slash)
			return (free_array_of_words(&dirs), NULL);
		full = ft_strjoin(slash, name);
		free(slash);
		if (!full)
			return (free_array_of_words(&dirs), NULL);
		if (access(full, X_OK) == 0)
		{
			free_array_of_words(&dirs);
			return (full);
		}
		free(full);
		i++;
	}
	free_array_of_words(&dirs);
	return (NULL);
}

/*
 * Run a single command (no pipeline): builtin runs in shell; external forks.
 * Redirections are applied in the process that runs the command (shell for
 * builtin, child for external).
 */
static int	run_single(t_command *cmd, char **env)
{
	pid_t	pid;
	char	*path;
	int		status;

	if (!cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]))
	{
		if (apply_redirections(cmd) != 0)
			return (g_exit_status = 1, 1);
		return (g_exit_status = run_builtin(cmd, &env), g_exit_status);
	}
	path = find_path(cmd->args[0], env);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (g_exit_status = 127, 127);
	}
	pid = fork();
	if (pid < 0)
		return (perror("fork"), free(path), g_exit_status = 1, 1);
	if (pid == 0)
	{
		if (apply_redirections(cmd) != 0)
			exit(1);
		execve(path, cmd->args, env);
		perror(cmd->args[0]);
		free(path);
		exit(127);
	}
	free(path);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
	return (g_exit_status);
}

/*
 * Count commands in pipeline.
 */
static int	count_commands(t_command *cmds)
{
	int	n;

	n = 0;
	while (cmds)
	{
		n++;
		cmds = cmds->next;
	}
	return (n);
}

/*
 * Run pipeline: N commands, N-1 pipes. Fork each command; connect stdout of
 * command i to stdin of command i+1. Wait for all; set g_exit_status to last.
 */
static int	run_pipeline(t_command *cmds, int n, char **env)
{
	(void)n;
	(void)env;
	/* TODO: allocate pipes, fork N children, in each child:
	   - dup2 pipe ends (stdin from prev, stdout to next)
	   - close all pipe fds
	   - apply_redirections(cur)
	   - if builtin run_builtin else find_path + execve
	   Parent: close all pipe fds, waitpid each child, set g_exit_status = last.
	   See EXECUTION_GUIDE.md Step 7. */
	return (run_single(cmds, env));
}

/*
 * Entry: run command list. If single command, run_single; else run_pipeline.
 */
int	execute(t_command *cmds, char **env)
{
	int	n;

	if (!cmds || !cmds->args || !cmds->args[0])
		return (0);
	n = count_commands(cmds);
	if (n == 1)
		return (run_single(cmds, env));
	return (run_pipeline(cmds, n, env));
}
