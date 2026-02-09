/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:58:34 by ymazzett          #+#    #+#             */
/*   Updated: 2025/10/08 14:55:17 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_input(char *input)
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

int	prompt_and_read(void)
{
	char	*line;

	g_signal = 0;
	line = readline(PROMPT);
	if (!line)
		return (1);
	if (g_signal != SIGINT && line[0] != '\0')
		add_history(line);
	handle_input(line);
	free(line);
	return (0);
}

int	main(void)
{
	struct sigaction	sa_int;
	int					error;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
	error = 0;
	while (!error)
		error = prompt_and_read();
	if (error == 1)
		return (1);
	return (0);
}
