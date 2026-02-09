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

static void	handle_input(char *input)
{
	char	**array_of_words;
	t_token	*tokens;

	array_of_words = ft_split(input, ' ');
	if (!array_of_words)
		return ;
	tokens = tokenize_all(array_of_words);
	if (tokens)
		free_tokens(&tokens);
	free_array_of_words(&array_of_words);
}

static int	prompt_and_read(void)
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
	handle_input(line);
	free(line);
	return (0);
}

int	shell_loop(void)
{
	int	error;

	error = 0;
	while (!error)
		error = prompt_and_read();
	return (error != 0);
}
