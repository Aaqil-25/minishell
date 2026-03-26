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

int	exec_is_builtin(char *name)
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

int	exec_run_builtin(t_command *cmd, char ***env, int last_status)
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
		return (builtin_exit(cmd->args, last_status));
	return (0);
}
