/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:48:51 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:48:54 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_internal.h"

static int	run_dot_builtin(t_command *cmd)
{
	if (!cmd->args[1])
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		return (2);
	}
	return (127);
}

static void	update_underscore_env(char ***env)
{
	char	*env_path;
	char	*tmp;

	env_path = exec_find_path("env", *env);
	if (!env_path)
		env_path = ft_strdup("env");
	if (!env_path)
		return ;
	tmp = ft_strjoin("_=", env_path);
	if (tmp)
	{
		export_set_assignment(tmp, env);
		free(tmp);
	}
	free(env_path);
}

static int	run_simple_builtin(t_command *cmd, char ***env)
{
	if (ft_strncmp(cmd->args[0], "help", 5) == 0)
		return (builtin_help(*env));
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
		return (update_underscore_env(env), builtin_env(cmd->args, *env));
	return (-1);
}

int	exec_is_builtin(char *name)
{
	if (!name)
		return (0);
	if (ft_strncmp(name, ".", 2) == 0)
		return (1);
	if (ft_strncmp(name, "help", 5) == 0)
		return (1);
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

int	exec_run_builtin(t_command *cmd, char ***env, int last_status)
{
	int	status;

	if (ft_strncmp(cmd->args[0], ".", 2) == 0)
		return (run_dot_builtin(cmd));
	status = run_simple_builtin(cmd, env);
	if (status >= 0)
		return (status);
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (builtin_exit(cmd->args, last_status));
	return (0);
}
