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

int input_via_pipe(char **env)
{
	char	*line;
	size_t	len;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (0);
		len = ft_strlen(line);
		line[len - 1] = '\0';
		handle_input(line, env);
		free(line);
	}
	return (0);
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