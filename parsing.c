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

static int	is_redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == APPEND || type == HEREDOC);
}

static t_redir	*add_redirection(t_redir *redir, t_redir *last_redir,
					t_token *op_token)
{
	t_redir	*current_redir;
	t_token	*target;

	target = op_token->next;
	if (!target || target->type != WORD)
	{
		ft_putstr_fd("minishell: syntax error near redirection\n", 2);
		return (NULL);
	}
	current_redir = malloc(sizeof(t_redir));
	if (!current_redir)
		return (NULL);
	current_redir->type = (t_redir_type)op_token->type;
	current_redir->filename = ft_strdup(target->value);
	if (!current_redir->filename)
	{
		free(current_redir);
		return (NULL);
	}
	current_redir->next = NULL;
	if (!redir)
		return (current_redir);
	last_redir->next = current_redir;
	return (current_redir);
}

static t_redir	*parse_redirection(t_redir *redir, t_redir **last_redir,
					t_token **current_token)
{
	t_redir	*new_redir;

	new_redir = add_redirection(redir, *last_redir, *current_token);
	if (!new_redir)
		return (NULL);
	if (!redir)
		redir = new_redir;
	*last_redir = new_redir;
	*current_token = (*current_token)->next->next;
	return (redir);
}

static t_command	*create_command(t_token **current_token)
{
	t_command	*cmd;
	t_redir		*last_redir;
	t_redir		*new_redirs;
	char		**new_args;
	int			has_content;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	last_redir = NULL;
	has_content = 0;
	while (*current_token && (*current_token)->type != PIPE)
	{
		if ((*current_token)->type == WORD)
		{
			new_args = append_to_array(cmd->args, (*current_token)->value);
			if (!new_args)
				return (free_commands(&cmd), NULL);
			cmd->args = new_args;
			has_content = 1;
			*current_token = (*current_token)->next;
		}
		else if (is_redirection((*current_token)->type))
		{
			new_redirs = parse_redirection(cmd->redirs, &last_redir,
					current_token);
			if (!new_redirs)
				return (free_commands(&cmd), NULL);
			cmd->redirs = new_redirs;
			has_content = 1;
		}
		else
			return (free_commands(&cmd), NULL);
	}
	if (!has_content)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (free_commands(&cmd), NULL);
	}
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
	t_command	*cmds;
	t_command	*current_cmd;

	if (!head || !*head)
		return (NULL);
	current_token = *head;
	if (current_token->type == PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (NULL);
	}
	cmds = NULL;
	while (current_token)
	{
		current_cmd = create_command(&current_token);
		if (!current_cmd)
			return (free_commands(&cmds), NULL);
		append_command(&cmds, current_cmd);
		if (current_token && current_token->type == PIPE)
		{
			if (!current_token->next || current_token->next->type == PIPE)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
					2);
				return (free_commands(&cmds), NULL);
			}
			current_token = current_token->next;
		}
	}
	return (cmds);
}
