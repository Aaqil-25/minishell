/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:01:15 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/25 20:19:50 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_subst(char *result, const char *s, size_t *i, char **env)
{
	char	*replacement;

	replacement = env_substitute(s, i, env);
	if (!replacement)
		return (free(result), NULL);
	result = env_append_part(result, replacement, ft_strlen(replacement));
	free(replacement);
	return (result);
}

static char	*expand_env_token(const char *s, char **env)
{
	char	*result;
	size_t	i;
	size_t	start;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	start = 0;
	while (s[i])
	{
		if (s[i] != '$')
			i++;
		else
		{
			result = env_append_part(result, s + start, i - start);
			if (!result)
				return (NULL);
			result = join_subst(result, s, &i, env);
			if (!result)
				return (NULL);
			start = i;
		}
	}
	return (env_append_part(result, s + start, i - start));
}

char	*parse_env_in_string(char **str, char **env)
{
	char	*expanded;

	expanded = expand_env_token(*str, env);
	if (!expanded)
		return (NULL);
	free(*str);
	*str = expanded;
	return (*str);
}
