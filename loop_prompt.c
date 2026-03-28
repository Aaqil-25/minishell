/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_prompt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 00:00:00 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/28 11:35:53 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*build_prompt(char **env)
{
	char	*username;
	char	*prompt;

	username = exec_get_env_value(env, "USER");
	if (!username)
		return (ft_strdup(PROMPT));
	username = ft_strjoin(username, "@");
	if (!username)
		return (ft_strdup(PROMPT));
	prompt = ft_strjoin(username, PROMPT);
	free(username);
	return (prompt);
}

static int	append_cont_line(char **line, char *cont)
{
	char	*tmp;
	char	*joined;

	tmp = *line;
	joined = ft_strjoin(tmp, "\n");
	if (!joined)
		return (free(tmp), free(cont), 1);
	*line = ft_strjoin(joined, cont);
	free(joined);
	if (!*line)
		return (free(tmp), free(cont), 1);
	free(tmp);
	free(cont);
	return (0);
}

static int	read_quote_continuation(char **line)
{
	char	*cont;

	while (has_unclosed_quotes(*line))
	{
		if (g_signal == SIGINT)
			return (free(*line), last_exit_status(130), 2);
		cont = readline("> ");
		if (g_signal == SIGINT)
			return (free(*line), free(cont), last_exit_status(130), 2);
		if (!cont)
			return (free(*line), 1);
		if (append_cont_line(line, cont))
			return (1);
	}
	return (0);
}

int	prompt_and_read(char ***env)
{
	char	*line;
	char	*prompt;
	int		status;

	prompt = build_prompt(*env);
	(term_apply(1), g_signal = 0);
	line = readline(prompt);
	(term_apply(0), free(prompt));
	if (!line)
		return (1);
	if (g_signal == SIGINT)
		return (free(line), last_exit_status(130), 0);
	status = read_quote_continuation(&line);
	if (status == 2)
		return (0);
	if (status)
		return (status);
	if (g_signal != SIGINT && line[0] != '\0')
		add_history(line);
	if (handle_input(line, env))
		return (free(line), 1);
	return (free(line), 0);
}

int	shell_loop(char ***env)
{
	int	error;

	error = 0;
	while (!error)
		error = prompt_and_read(env);
	return (error);
}
