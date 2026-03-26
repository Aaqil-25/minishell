/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:01:43 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/25 21:02:33 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	redir_name_len(const char *s)
{
	size_t	i;
	size_t	len;
	char	quote;

	i = 0;
	len = 0;
	quote = 0;
	while (s[i])
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i];
		else if (quote && s[i] == quote && (i == 0 || s[i - 1] != '\\'))
			quote = 0;
		else
			len++;
		i++;
	}
	return (len);
}

static char	*normalize_redir_name(const char *s)
{
	char	*name;
	size_t	i;
	size_t	j;
	char	quote;

	name = (char *)malloc(redir_name_len(s) + 1);
	if (!name)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (s[i])
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i];
		else if (quote && s[i] == quote && (i == 0 || s[i - 1] != '\\'))
			quote = 0;
		else
			name[j++] = s[i];
		i++;
	}
	name[j] = '\0';
	return (name);
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
	current_redir->filename = normalize_redir_name(target->value);
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
