/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:46:30 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/26 16:34:09 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input(char *input, char ***env)
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
	(void)execute(cmds, env);
	free_commands(&cmds);
}

static int	prompt_and_read(char ***env)
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

int	shell_loop(char ***env)
{
	int	error;

	error = 0;
	while (!error)
		error = prompt_and_read(env);
	return (error != 0);
}
