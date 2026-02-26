/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:04:23 by ymazzett          #+#    #+#             */
/*   Updated: 2026/02/26 15:59:10 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*add_redirection(t_redir *redir, t_redir *last_redir,
					t_token **current_token)
{
	t_redir	*current_redir;

	if (!(*current_token)->next || (*current_token)->next->type != WORD)
	{
		write(2, "minishell: syntax error near redirection\n", 42);
		return (NULL);
	}
	current_redir = malloc(sizeof(t_redir));
	if (!current_redir)
		return (NULL);
	current_redir->type = (t_redir_type)(*current_token)->type;
	current_redir->filename = ft_strdup((*current_token)->next->value);
	current_redir->next = NULL;
	if (!redir)
		return (current_redir);
	last_redir->next = current_redir;
	return (current_redir);
}

t_redir	*create_redirections(t_token **current_token)
{
	t_redir	*redir;
	t_redir	*last_redir;

	redir = NULL;
	last_redir = NULL;
	while (*current_token && (*current_token)->type != PIPE)
	{
		if ((*current_token)->type != WORD && (*current_token)->type != PIPE)
		{
			last_redir = add_redirection(redir, last_redir, current_token);
			if (!last_redir)
			{
				// Free any redirections created so far
				// (You should add a free_redirs function)
				return (NULL);
			}
			if (!redir)
				redir = last_redir;
			*current_token = (*current_token)->next;
		}
		*current_token = (*current_token)->next;
	}
	return (redir);
}

t_command	*create_command(t_token **current_token)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	while (*current_token && (*current_token)->type == WORD)
	{
		cmd->args = append_to_array(cmd->args, (*current_token)->value);
		*current_token = (*current_token)->next;
	}
	cmd->redirs = create_redirections(current_token);
	return (cmd);
}

void	append_command(t_command **cmds, t_command *new_cmd)
{
	t_command	*last_cmd;

	if (!*cmds)
	{
		*cmds = new_cmd;
		return ;
	}
	last_cmd = *cmds;
	while (last_cmd->next)
		last_cmd = last_cmd->next;
	last_cmd->next = new_cmd;
	new_cmd->prev = last_cmd;
}

t_command	*parser(t_token **head)
{
	t_token		*current_token;
	t_command	*cmd;
	t_command	*current_cmd;

	if (!head || !*head)
		return (NULL);
	current_token = *head;
	cmd = NULL;
	while (current_token)
	{
		current_cmd = create_command(&current_token);
		if (!current_cmd)
			return (NULL);
		append_command(&cmd, current_cmd);
		if (current_token && current_token->type == PIPE)
			current_token = current_token->next;
	}
	return (cmd);
}
