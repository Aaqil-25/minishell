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

int	run_single(t_command *cmd, char ***env, int last_status)
{
	if (!cmd->args || !cmd->args[0])
		return (last_status);
	if (exec_is_builtin(cmd->args[0]))
		return (run_builtin_cmd(cmd, env, last_status));
	return (run_external_cmd(cmd, *env));
}

int	execute(t_command *cmds, char ***env)
{
	t_command	*cur;
	int			n;
	int			status;

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
	{
		status = run_single(cmds, env, 0);
		last_exit_status(status);
		return (status);
	}
	return (run_pipeline(cmds, n, *env, 0));
}
