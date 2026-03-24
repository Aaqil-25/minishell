/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:01:15 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/24 20:29:46 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse_env_in_string(char **str, char **env)
{
	char	*env_value;

	if (ft_strncmp(*str, "$?", 2) == 0)
	{
		env_value = ft_itoa(last_exit_status(-1));
		free(*str);
		*str = env_value;
		return (*str);
	}
	env_value = exec_get_env_value(env, *str + 1);
	if (!env_value)
		env_value = "";
	free(*str);
	*str = ft_strdup(env_value);
	return (*str);
}

void	parse_env_variable(t_command *cmds, char **env)
{
	t_command	*current_cmd;
	int			i;

	current_cmd = cmds;
	while (current_cmd)
	{
		i = 0;
		while (current_cmd->args && current_cmd->args[i])
		{
			if (ft_strchr(current_cmd->args[i], '$'))
				current_cmd->args[i] = parse_env_in_string\
(&(current_cmd->args[i]), env);
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
