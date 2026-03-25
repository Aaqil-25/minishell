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

static char	*read_stdin_line(void)
{
	char	*line;
	char	*tmp;
	char	c;
	ssize_t	n;
	size_t	len;
	size_t	cap;

	cap = 64;
	len = 0;
	line = malloc(cap);
	if (!line)
		return (NULL);
	n = 0;
	while ((n = read(STDIN_FILENO, &c, 1)) == 1 && c != '\n')
	{
		if (len + 1 >= cap)
		{
			tmp = ft_calloc(cap * 2, 1);
			if (!tmp)
				return (free(line), NULL);
			ft_memcpy(tmp, line, len);
			free(line);
			line = tmp;
			cap *= 2;
		}
		line[len++] = c;
	}
	if (n <= 0 && len == 0)
		return (free(line), NULL);
	line[len] = '\0';
	return (line);
}

int input_via_pipe(char **env)
{
	char	*line;

	line = read_stdin_line();
	if (!line)
		return (last_exit_status(-1));
	handle_input(line, env);
	free(line);
	return (last_exit_status(-1));
}

int	main(int argc, char **argv, char **envp)
{

	(void)argc;
	(void)argv;
	signals_setup();
	if (!isatty(STDIN_FILENO))
	{
		return (input_via_pipe(envp));
	}
	return (shell_loop(envp));
}

/*
int	main(void)
{
	signals_setup();
	return (shell_loop());
}
*/