/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:48:39 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:48:41 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_INTERNAL_H
# define EXEC_INTERNAL_H

# include "minishell.h"

typedef struct child_process_params
{
	t_command	*cmd;
	int			i;
	int			n;
	int			prev[2];
	int			curr[2];
}	t_child_process_params;

int		exec_apply_redirections(t_command *cmd);
char	*exec_get_env_value(char **env, char *name);
char	*exec_find_path(char *name, char **env);
int		exec_is_builtin(char *name);
int		exec_run_builtin(t_command *cmd, char ***env, int last_status);
int		run_single(t_command *cmd, char **env, int last_status);
int		run_pipeline(t_command *cmds, int n, char **env, int last_status);

// Pipeline helpers

pid_t	*allocate_pids(int n);
void	close_pipe_ends(int fd0, int fd1);
void	setup_child_redirections(int i, int n, int *prev, int *curr);
void	update_prev_pipe(int i, int n, int *prev, int *curr);
void	wait_for_all(pid_t *pids, int n);
void	create_next_pipe(int i, int n, int *pipefd);
pid_t	fork_child(t_child_process_params params, char **env, int last);

#endif
