/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:45:06 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:45:06 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lexer(char *line)
{
	char	**array_of_words;
	t_token	*tokens;

	array_of_words = wd_n_quotes_split(line);
	if (!array_of_words || !array_of_words[0])
	{
		if (array_of_words)
			free_array_of_words(&array_of_words);
		return (NULL);
	}
	tokens = tokenize_all(array_of_words);
	free_array_of_words(&array_of_words);
	return (tokens);
}
/* ************************************************************************** */

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
