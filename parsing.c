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

static int	parse_and_append(t_token **current_token, t_command **cmds)
{
	t_command	*current_cmd;

	current_cmd = create_command(current_token);
	if (!current_cmd)
		return (0);
	append_command(cmds, current_cmd);
	return (1);
}

static int	advance_after_pipe(t_token **current_token)
{
	if (!*current_token || (*current_token)->type != PIPE)
		return (1);
	if (!(*current_token)->next || (*current_token)->next->type == PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}
	*current_token = (*current_token)->next;
	return (1);
}

t_command	*parser(t_token **head)
{
	t_token		*current_token;
	t_command	*cmds;

	if (!head || !*head)
		return (NULL);
	current_token = *head;
	cmds = NULL;
	while (current_token)
	{
		if (!parse_and_append(&current_token, &cmds))
			return (free_commands(&cmds), NULL);
		if (!advance_after_pipe(&current_token))
			return (free_commands(&cmds), NULL);
	}
	return (cmds);
}
