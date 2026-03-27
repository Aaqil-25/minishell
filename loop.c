/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:46:30 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/03/25 19:42:32 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input(char *input, char ***env)
{
	t_token		*tokens;
	t_command	*cmds;
	int			status;

	tokens = lexer(input);
	if (!tokens)
		return (0);
	cmds = parser(&tokens);
	free_tokens(&tokens);
	if (!cmds)
		return (0);
	parse_env_variable(cmds, *env);
	status = execute(cmds, env);
	free_commands(&cmds);
	if (builtin_exit_requested(-1))
		return (1);
	(void)status;
	return (0);
}

static char	*build_prompt(char **env)
{
	char	*username;
	char	*prompt;

	username = exec_get_env_value(env, "USER");
	if (username)
	{
		username = ft_strjoin(username, "@");
		prompt = ft_strjoin(username, PROMPT);
		free(username);
	}
	else
		prompt = ft_strdup(PROMPT);
	return (prompt);
}

int	prompt_and_read(char ***env)
{
	char	*line;
	char	*prompt;
	char	*cont;
	char	*tmp;
	char	*joined;

	prompt = build_prompt(*env);
	(term_apply(1), g_signal = 0);
	line = readline(prompt);
	(term_apply(0), free(prompt));
	if (!line)
		return (1);
	while (has_unclosed_quotes(line))
	{
		cont = readline("> ");
		if (!cont)
			return ((void)free(line), 1);
		tmp = line;
		joined = ft_strjoin(tmp, "\n");
		if (!joined)
			return (free(tmp), free(cont), 1);
		line = ft_strjoin(joined, cont);
		free(joined);
		if (!line)
			return (free(tmp), free(cont), 1);
		(free(tmp), free(cont));
	}
	if (g_signal != SIGINT && line[0] != '\0')
		add_history(line);
	if (handle_input(line, env))
		return (free(line), 1);
	free(line);
	return (0);
}

int	shell_loop(char ***env)
{
	int	error;

	error = 0;
	while (!error)
		error = prompt_and_read(env);
	return (error);
}
