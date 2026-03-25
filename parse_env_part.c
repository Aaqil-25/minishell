/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env_part.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:01:15 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/25 20:19:50 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*env_append_part(char *base, const char *part, size_t part_len)
{
	char	*new_str;
	size_t	base_len;

	base_len = 0;
	if (base)
		base_len = ft_strlen(base);
	new_str = (char *)malloc(base_len + part_len + 1);
	if (!new_str)
		return (free(base), NULL);
	if (base)
	{
		ft_memcpy(new_str, base, base_len);
		free(base);
	}
	if (part_len > 0)
		ft_memcpy(new_str + base_len, part, part_len);
	new_str[base_len + part_len] = '\0';
	return (new_str);
}

static char	*substitute_exit_code(size_t *i)
{
	*i += 2;
	return (ft_itoa(last_exit_status(-1)));
}

static char	*substitute_env_name(const char *s, size_t *i, char **env)
{
	char	*name;
	char	*value;
	size_t	len;

	len = 0;
	while (is_env_char(s[*i + 1 + len]))
		len++;
	if (len == 0)
	{
		*i += 1;
		return (ft_strdup("$"));
	}
	name = ft_substr(s, *i + 1, len);
	if (!name)
		return (NULL);
	value = exec_get_env_value(env, name);
	free(name);
	*i += len + 1;
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*env_substitute(const char *s, size_t *i, char **env)
{
	if (s[*i + 1] == '?')
		return (substitute_exit_code(i));
	return (substitute_env_name(s, i, env));
}
