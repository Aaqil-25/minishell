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

static int	is_escaped_char(char *s, size_t i)
{
	int	count;

	count = 0;
	while (i > 0 && s[i - 1] == '\\')
	{
		count++;
		i--;
	}
	return (count % 2);
}

static int	handle_segment(char *segment, char ***env)
{
	t_token		*tokens;
	t_command	*cmds;
	int			status;

	tokens = lexer(segment);
	if (!tokens)
		return (0);
	cmds = parser(&tokens);
	free_tokens(&tokens);
	if (!cmds)
		return (0);
	parse_env_variable(cmds, *env);
	status = execute(cmds, env);
	free_commands(&cmds);
	if (builtin_exit_requested(-1))
		return (1);
	(void)status;
	return (0);
}

static int	process_segment(char *input, size_t start, size_t end, char ***env)
{
	char	*segment;
	char	*trimmed;

	segment = ft_substr(input, start, end - start);
	if (!segment)
		return (-1);
	trimmed = ft_strtrim(segment, " \t\n");
	free(segment);
	if (!trimmed)
		return (-1);
	if (trimmed[0] != '#' && trimmed[0] && handle_segment(trimmed, env))
		return (free(trimmed), 1);
	free(trimmed);
	return (0);
}

static int	is_segment_end(char *input, size_t i, char *quote)
{
	if (input[i] && !is_escaped_char(input, i)
		&& (input[i] == '\'' || input[i] == '"'))
	{
		if (!*quote)
			*quote = input[i];
		else if (*quote == input[i])
			*quote = 0;
	}
	if ((input[i] == ';' && !*quote && !is_escaped_char(input, i))
		|| input[i] == '\0')
		return (1);
	return (0);
}

int	handle_input(char *input, char ***env)
{
	size_t	i;
	size_t	start;
	char	quote;
	int		status;

	i = 0;
	start = 0;
	quote = 0;
	while (1)
	{
		if (is_segment_end(input, i, &quote))
		{
			status = process_segment(input, start, i, env);
			if (status != 0)
				return (status == 1);
			if (input[i] == '\0')
				break ;
			start = i + 1;
		}
		i++;
	}
	return (0);
}
