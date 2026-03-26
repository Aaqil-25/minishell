/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:01:15 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/25 20:19:50 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	arg_skip_quote(char *arg, size_t *i, char *quote)
{
	if (!*quote && (arg[*i] == '\'' || arg[*i] == '"'))
	{
		*quote = arg[*i];
		(*i)++;
		return (1);
	}
	if (*quote && arg[*i] == *quote && (*i == 0 || arg[*i - 1] != '\\'))
	{
		*quote = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

static char	*arg_handle_dollar(char *new_arg, char *arg, size_t *i, char **env)
{
	char	*replacement;

	replacement = env_substitute(arg, i, env);
	if (!replacement)
		return (free(new_arg), NULL);
	new_arg = env_append_part(new_arg, replacement, ft_strlen(replacement));
	free(replacement);
	return (new_arg);
}

static char	*arg_append_one_char(char *new_arg, char *arg, size_t *i)
{
	new_arg = env_append_part(new_arg, arg + *i, 1);
	if (!new_arg)
		return (NULL);
	(*i)++;
	return (new_arg);
}

static char	*parse_arg_by_quote(char *arg, char **env)
{
	char	*new_arg;
	char	quote;
	size_t	i;

	new_arg = ft_strdup("");
	if (!new_arg)
		return (NULL);
	quote = 0;
	i = 0;
	while (arg[i])
	{
		if (arg_skip_quote(arg, &i, &quote))
			continue ;
		if (arg[i] == '$' && quote != '\'')
		{
			new_arg = arg_handle_dollar(new_arg, arg, &i, env);
			if (!new_arg)
				return (NULL);
			continue ;
		}
		new_arg = arg_append_one_char(new_arg, arg, &i);
		if (!new_arg)
			return (NULL);
	}
	return (new_arg);
}

void	parse_env_variable(t_command *cmds, char **env)
{
	t_command	*current_cmd;
	char		*new_arg;
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
