/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
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

static char	*append_part(char *base, const char *part, size_t part_len)
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

static char	*env_replacement(const char *s, size_t *i, char **env)
{
	char	*name;
	char	*value;
	size_t	len;

	if (s[*i + 1] == '?')
	{
		*i += 2;
		return (ft_itoa(last_exit_status(-1)));
	}
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

static char	*expand_env_token(const char *s, char **env)
{
	char	*result;
	char	*replacement;
	size_t	i;
	size_t	start;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	start = 0;
	while (s[i])
	{
		if (s[i] == '$')
		{
			result = append_part(result, s + start, i - start);
			if (!result)
				return (NULL);
			replacement = env_replacement(s, &i, env);
			if (!replacement)
				return (free(result), NULL);
			result = append_part(result, replacement, ft_strlen(replacement));
			free(replacement);
			if (!result)
				return (NULL);
			start = i;
		}
		else
			i++;
	}
	return (append_part(result, s + start, i - start));
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

static char	*parse_arg_by_quote(char *arg, char **env)
{
	char	*new_arg;
	char	*replacement;
	char	quote;
	size_t	i;

	new_arg = ft_strdup("");
	if (!new_arg)
		return (NULL);
	quote = 0;
	i = 0;
	while (arg[i])
	{
		if (!quote && (arg[i] == '\'' || arg[i] == '"'))
		{
			quote = arg[i++];
			continue ;
		}
		if (quote && arg[i] == quote && (i == 0 || arg[i - 1] != '\\'))
		{
			quote = 0;
			i++;
			continue ;
		}
		if (arg[i] == '$' && quote != '\'')
		{
			replacement = env_replacement(arg, &i, env);
			if (!replacement)
				return (free(new_arg), NULL);
			new_arg = append_part(new_arg, replacement, ft_strlen(replacement));
			free(replacement);
			if (!new_arg)
				return (NULL);
			continue ;
		}
		new_arg = append_part(new_arg, arg + i, 1);
		if (!new_arg)
			return (NULL);
		i++;
	}
	return (new_arg);
}

void	parse_env_variable(t_command *cmds, char **env)
{
	t_command	*current_cmd;
	char	*new_arg;
	int			i;

	current_cmd = cmds;
	while (current_cmd)
	{
		i = 0;
		while (current_cmd->args && current_cmd->args[i])
		{
			new_arg = parse_arg_by_quote(current_cmd->args[i], env);
			if (new_arg)
			{
				free(current_cmd->args[i]);
				current_cmd->args[i] = new_arg;
			}
			i++;
		}
		current_cmd = current_cmd->next;
	}
}

int	parse_word_token(t_command *cmd, t_token **current_token)
{
	char	**new_args;

	new_args = append_to_array(cmd->args, (*current_token)->value);
	if (!new_args)
		return (0);
	cmd->args = new_args;
	*current_token = (*current_token)->next;
	return (1);
}

int	parse_command_tokens(t_command *cmd, t_token **current_token,
			t_redir **last_redir)
{
	t_redir	*new_redirs;

	while (*current_token && (*current_token)->type != PIPE)
	{
		if ((*current_token)->type == WORD)
		{
			if (!parse_word_token(cmd, current_token))
				return (0);
		}
		else if (is_redirection((*current_token)->type))
		{
			new_redirs = parse_redirection(cmd->redirs, last_redir,
					current_token);
			if (!new_redirs)
				return (0);
			cmd->redirs = new_redirs;
		}
		else
			return (0);
	}
	return (1);
}

t_command	*create_command(t_token **current_token)
{
	t_command	*cmd;
	t_redir		*last_redir;

	cmd = init_command();
	if (!cmd)
		return (NULL);
	last_redir = NULL;
	if (!parse_command_tokens(cmd, current_token, &last_redir))
		return (free_commands(&cmd), NULL);
	if (!cmd->args && !cmd->redirs)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (free_commands(&cmd), NULL);
	}
	return (cmd);
}
