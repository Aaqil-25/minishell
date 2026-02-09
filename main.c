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
#include <termios.h>

volatile sig_atomic_t	g_signal = 0;

static struct termios	g_term_orig;
static int				g_term_orig_set = 0;

static void	term_restore(void)
{
	if (g_term_orig_set)
		tcsetattr(STDIN_FILENO, TCSANOW, &g_term_orig);
}

static void	term_init(void)
{
	if (g_term_orig_set)
		return ;
	if (tcgetattr(STDIN_FILENO, &g_term_orig) != 0)
		return ;
	g_term_orig_set = 1;
}

/*
 * Bash-like UX: don't print "^C" / "^\\" when pressing control keys.
 * This is controlled by the terminal flag ECHOCTL.
 *
 * Additionally: while sitting at the prompt, bash treats Ctrl+\ as a no-op.
 * The 42-allowed way to get that behaviour is to disable the terminal "quit"
 * special character (VQUIT) while reading input, and ignore SIGQUIT.
 */
static void	term_apply(int prompt_mode)
{
	struct termios	t;

	term_init();
	if (!g_term_orig_set)
		return ;
	t = g_term_orig;
#ifdef ECHOCTL
	t.c_lflag &= ~(ECHOCTL);
#endif
	if (prompt_mode)
	{
#ifdef VQUIT
# ifdef _POSIX_VDISABLE
		t.c_cc[VQUIT] = _POSIX_VDISABLE;
# endif
#endif
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

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

int	main(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	int					error;

	term_apply(1);
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
	error = 0;
	while (!error)
		error = prompt_and_read();
	term_restore();
	if (error == 1)
		return (1);
	return (0);
}
