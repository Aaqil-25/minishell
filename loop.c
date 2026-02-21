/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:46:30 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:46:33 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input(char *input, char ***env)
{
	t_command	cmd;
	char		**array_of_words;

	array_of_words = ft_split(input, ' ');
	if (!array_of_words || !array_of_words[0])
	{
		if (array_of_words)
			free_array_of_words(&array_of_words);
		return ;
	}
	cmd.args = array_of_words;
	cmd.redirs = NULL;
	cmd.next = NULL;
	cmd.prev = NULL;
	(void)execute(&cmd, env);
	free_array_of_words(&array_of_words);
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
