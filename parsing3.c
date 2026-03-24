/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:01:43 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/24 21:49:07 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int last_exit_status(int new_status)
{
	static int	status = 0;

	if (new_status != -1)
		status = new_status;
	// printf("Last exit status updated to: %d\n", status);
	return (status);
}

int	is_redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == APPEND || type == HEREDOC);
}

t_redir	*add_redirection(t_redir *redir, t_redir *last_redir,
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

t_redir	*parse_redirection(t_redir *redir, t_redir **last_redir,
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

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}
