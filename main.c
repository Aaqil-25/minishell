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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	size_t	len;

	(void)argc;
	(void)argv;
	signals_setup();
	if (!isatty(STDIN_FILENO))
	{
		line = get_next_line(STDIN_FILENO);
		len = ft_strlen(line);
		line[len - 1] = '\0';
		if (!line)
			return (1);
		handle_input(line, envp);
		free(line);
		return (0);
	}
	else
		return (shell_loop(envp));
}

/*
int	main(void)
{
	signals_setup();
	return (shell_loop());
}
*/