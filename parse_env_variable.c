/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env_variable.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 00:00:00 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/27 14:34:45 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse_arg_by_quote(char *arg, char **env);

static int	keep_expanded_arg(char *old_arg, char *new_arg)
{
	if (new_arg[0] != '\0')
		return (1);
	if (ft_strchr(old_arg, '\'') || ft_strchr(old_arg, '"'))
		return (1);
	return (0);
}

void	parse_env_variable(t_command *cmds, char **env)
{
	t_command	*cmd;
	char		*new_arg;
	int			i;
	int			j;

	cmd = cmds;
	while (cmd)
	{
		i = 0;
		j = 0;
		while (cmd->args && cmd->args[i])
		{
			new_arg = parse_arg_by_quote(cmd->args[i], env);
			if (!new_arg)
				cmd->args[j++] = cmd->args[i];
			else if (keep_expanded_arg(cmd->args[i], new_arg))
				(free(cmd->args[i]), cmd->args[j++] = new_arg);
			else
				(free(cmd->args[i]), free(new_arg));
			i++;
		}
		if (cmd->args)
			cmd->args[j] = NULL;
		cmd = cmd->next;
	}
}
