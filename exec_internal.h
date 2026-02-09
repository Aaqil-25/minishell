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

int		exec_apply_redirections(t_command *cmd);
char	*exec_get_env_value(char **env, char *name);
char	*exec_find_path(char *name, char **env);
int		exec_is_builtin(char *name);
int		exec_run_builtin(t_command *cmd, char ***env, int last_status);

#endif
