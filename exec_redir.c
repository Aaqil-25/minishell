/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:48:24 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:48:26 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_internal.h"
#include <fcntl.h>

static int	open_and_dup(char *file, int flags, int target)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd < 0)
		return (perror(file), -1);
	dup2(fd, target);
	close(fd);
	return (0);
}

static int	fill_heredoc_pipe(char *delimiter, int write_fd, char **env)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		parse_env_in_string(&line, env);
		ft_putstr_fd(line, write_fd);
		ft_putchar_fd('\n', write_fd);
		free(line);
	}
	return (0);
}

static int	apply_heredoc(char *delimiter, char **env)
{
	int	pipefd[2];

	if (pipe(pipefd) < 0)
		return (perror("pipe"), -1);
	fill_heredoc_pipe(delimiter, pipefd[1], env);
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		close(pipefd[0]);
		return (perror("dup2"), -1);
	}
	close(pipefd[0]);
	return (0);
}

int	exec_apply_redirections(t_command *cmd, char **env)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == C_REDIR_IN
			&& open_and_dup(r->filename, O_RDONLY, STDIN_FILENO) != 0)
			return (-1);
		if (r->type == C_REDIR_OUT
			&& open_and_dup(r->filename, O_WRONLY | O_CREAT | O_TRUNC,
				STDOUT_FILENO) != 0)
			return (-1);
		if (r->type == C_REDIR_APPEND
			&& open_and_dup(r->filename, O_WRONLY | O_CREAT | O_APPEND,
				STDOUT_FILENO) != 0)
			return (-1);
		if (r->type == C_REDIR_HEREDOC
			&& apply_heredoc(r->filename, env) != 0)
			return (-1);
		r = r->next;
	}
	return (0);
}
