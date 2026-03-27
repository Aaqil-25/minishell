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

void	handle_input(char *input, char ***env)
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
	parse_env_variable(cmds, *env);
	(void)execute(cmds, env);
	free_commands(&cmds);
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

	prompt = build_prompt(*env);
	term_apply(1);
	g_signal = 0;
	line = readline(prompt);
	term_apply(0);
	free(prompt);
	if (!line)
		return (1);
	if (g_signal != SIGINT && line[0] != '\0')
		add_history(line);
	handle_input(line, env);
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
