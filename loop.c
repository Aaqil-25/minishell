/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:46:30 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/23 16:14:06 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse_env_in_string(char **str, char **env)
{
	char	*env_value;

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

void	handle_input(char *input, char **env)
{
	t_token		*tokens;
	t_command	*cmds;

	tokens = lexer(input);
	if (!tokens)
		return ;
	cmds = parser(&tokens);
	free_tokens(&tokens);
	if (!cmds)
		return ;
	parse_env_variable(cmds, env);
	(void)execute(cmds, env);
	free_commands(&cmds);
}

int	prompt_and_read(char **env)
{
	char	*line;

	term_apply(1);
	g_signal = 0;
	line = readline(PROMPT);
	term_apply(0);
	if (!line)
		return (1);
	if (g_signal != SIGINT && line[0] != '\0')
		add_history(line);
	handle_input(line, env);
	free(line);
	return (0);
}

int	shell_loop(char **env)
{
	int	error;

	error = 0;
	while (!error)
		error = prompt_and_read(env);
	return (error);
}
