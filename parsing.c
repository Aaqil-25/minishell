/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:04:23 by ymazzett          #+#    #+#             */
/*   Updated: 2026/02/23 14:26:03 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parser(t_token **head)
{
	t_token		*current;
	t_command	*cmd;

	if (!head || !*head)
		return (NULL);
	current = *head;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	while (current)
	{
		if (current->type == WORD)
		{
			cmd->args = append_to_array(cmd->args, current->value);
		}
		current = current->next;
	}
	return (cmd);
}
