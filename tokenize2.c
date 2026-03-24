/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:44:59 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/03/24 20:00:04 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*create_token_with_quotes(char *value, t_token_type type)
{ // POINTER ARGUMENT IS MISSING!
	t_token	*new_token;
	char	*stripped_value;

	stripped_value = strip_quotes(value);
	if (!stripped_value)
		return (NULL);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		free(stripped_value);
		return (NULL);
	}
	new_token->value = stripped_value;
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

static t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new_token;

	if (!value)
		return (NULL);
	if (type == WORD && (value[0] == '"' || value[0] == '\''))
		return (create_token_with_quotes(value, type));
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

static t_token_type	get_token_type(char *word)
{
	size_t	len;

	len = ft_strlen(word);
	if (!ft_strncmp(word, "|", len))
		return (PIPE);
	else if (!ft_strncmp(word, ">", len))
		return (REDIR_OUT);
	else if (!ft_strncmp(word, "<", len))
		return (REDIR_IN);
	else if (!ft_strncmp(word, ">>", len))
		return (APPEND);
	else if (!ft_strncmp(word, "<<", len))
		return (HEREDOC);
	else
		return (WORD);
}

t_token	*tokenize_all(char **array_of_words)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	int		i;

	if (!array_of_words || !array_of_words[0])
		return (NULL);
	head = create_token(array_of_words[0], get_token_type(array_of_words[0]));
	if (!head)
		return (NULL);
	current = head;
	i = 1;
	while (array_of_words[i])
	{
		new_token = create_token(array_of_words[i],
				get_token_type(array_of_words[i]));
		if (!new_token)
			return (head);
		current->next = new_token;
		current = new_token;
		i++;
	}
	return (head);
}
