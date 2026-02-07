/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 12:10:43 by codespace         #+#    #+#             */
/*   Updated: 2025/10/08 12:10:44 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lexer(char *line)
{
	(void)line;
	return (NULL);
}

void	free_tokens(t_token **head)
{
	t_token	*cur;
	t_token	*next;

	if (!head || !*head)
		return ;
	cur = *head;
	while (cur)
	{
		next = cur->next;
		free(cur->value);
		free(cur);
		cur = next;
	}
	*head = NULL;
}

t_command	*parser(t_token **head)
{
	(void)head;
	return (NULL);
}

static void	free_redirs(t_redir *r)
{
	t_redir	*next;

	while (r)
	{
		next = r->next;
		free(r->filename);
		free(r);
		r = next;
	}
}

void	free_commands(t_command **head)
{
	t_command	*cur;
	t_command	*next;
	int			i;

	if (!head || !*head)
		return ;
	cur = *head;
	while (cur)
	{
		next = cur->next;
		if (cur->args)
		{
			i = 0;
			while (cur->args[i])
				free(cur->args[i++]);
			free(cur->args);
		}
		free_redirs(cur->redirs);
		free(cur);
		cur = next;
	}
	*head = NULL;
}
