/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:58:34 by ymazzett          #+#    #+#             */
/*   Updated: 2025/10/08 14:55:17 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	grow_and_put_char(char **line, size_t *len, size_t *cap, char c)
{
	char	*tmp;

	if (*len + 1 >= *cap)
	{
		tmp = ft_calloc(*cap * 2, 1);
		if (!tmp)
			return (free(*line), 0);
		ft_memcpy(tmp, *line, *len);
		free(*line);
		*line = tmp;
		*cap *= 2;
	}
	(*line)[*len] = c;
	(*len)++;
	return (1);
}

static char	*read_stdin_line(void)
{
	char	*line;
	ssize_t	n;
	char	c;
	size_t	len;
	size_t	cap;

	cap = 64;
	len = 0;
	line = malloc(cap);
	if (!line)
		return (NULL);
	n = read(STDIN_FILENO, &c, 1);
	while (n == 1 && c != '\n')
	{
		if (!grow_and_put_char(&line, &len, &cap, c))
			return (NULL);
		n = read(STDIN_FILENO, &c, 1);
	}
	if (n <= 0 && len == 0)
		return (free(line), NULL);
	line[len] = '\0';
	return (line);
}

int	input_via_pipe(char ***env)
{
	char	*line;

	line = read_stdin_line();
	if (!line)
		return (last_exit_status(-1));
	(void)handle_input(line, env);
	free(line);
	return (last_exit_status(-1));
}
