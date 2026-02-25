/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:04:23 by ymazzett          #+#    #+#             */
/*   Updated: 2026/02/25 17:17:39 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_command(t_token **current)
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
	while (*current && (*current)->type == WORD)
	{
		cmd->args = append_to_array(cmd->args, (*current)->value);
		*current = (*current)->next;
	}
	return (cmd);
}

t_command	*parser(t_token **head)
{
	t_token		*current;
	t_command	*cmd;

	if (!head || !*head)
		return (NULL);
	current = *head;
	while (current)
	{
		cmd = create_command(&current);
		if (current && current->type != WORD)
		{
			// Something
		}
		if (current->next)
			current = current->next;
	}
	return (cmd);
}
