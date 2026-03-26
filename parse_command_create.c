/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_create.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:01:15 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/25 20:19:50 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_word_token(t_command *cmd, t_token **current_token)
{
	char	**new_args;

	new_args = append_to_array(cmd->args, (*current_token)->value);
	if (!new_args)
		return (0);
	cmd->args = new_args;
	*current_token = (*current_token)->next;
	return (1);
}

int	parse_command_tokens(t_command *cmd, t_token **current_token,
		t_redir **last_redir)
{
	t_redir	*new_redirs;

	while (*current_token && (*current_token)->type != PIPE)
	{
		if ((*current_token)->type == WORD)
		{
			if (!parse_word_token(cmd, current_token))
				return (0);
		}
		else if (is_redirection((*current_token)->type))
		{
			new_redirs = parse_redirection(cmd->redirs, last_redir,
					current_token);
			if (!new_redirs)
				return (0);
			cmd->redirs = new_redirs;
		}
		else
			return (0);
	}
	return (1);
}

t_command	*create_command(t_token **current_token)
{
	t_command	*cmd;
	t_redir		*last_redir;

	cmd = init_command();
	if (!cmd)
		return (NULL);
	last_redir = NULL;
	if (!parse_command_tokens(cmd, current_token, &last_redir))
		return (free_commands(&cmd), NULL);
	if (!cmd->args && !cmd->redirs)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (free_commands(&cmd), NULL);
	}
	return (cmd);
}
