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
#include <errno.h>


static void	cleanup_shell(char **env)
{
	int	i;

	if (!env)
	{
		rl_clear_history();
		return ;
	}
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
	rl_clear_history();
}
static char	*build_env_entry(char *key, int value)
{
	char	*num;
	char	*tmp;
	char	*entry;

	num = ft_itoa(value);
	if (!num)
		return (NULL);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (free(num), NULL);
	entry = ft_strjoin(tmp, num);
	free(num);
	free(tmp);
	return (entry);
}

static char	**dup_env(char **envp)
{
	char	**env;
	int		i;

	env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		env = append_to_array(env, envp[i]);
		if (!env)
			return (NULL);
		i++;
	}
	if (!env)
		env = ft_calloc(1, sizeof(char *));
	return (env);
}

static int	set_env_var(char ***env, char *key, int value)
{
	char	*entry;
	size_t	len;
	char	**new_env;
	int		i;

	entry = build_env_entry(key, value);
	if (!entry)
		return (0);
	len = ft_strlen(key);
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, len) == 0 && (*env)[i][len] == '=')
			return (free((*env)[i]), (*env)[i] = entry, 1);
		i++;
	}
	new_env = append_to_array(*env, entry);
	free(entry);
	if (!new_env)
		return (0);
	*env = new_env;
	return (1);
}

static void	init_shlvl(char ***env)
{
	char	*val;
	char	*end;
	long	lvl;
	long	next;

	val = exec_get_env_value(*env, "SHLVL");
	if (!val)
		return ((void)set_env_var(env, "SHLVL", 1));
	errno = 0;
	lvl = strtol(val, &end, 10);
	if (!val[0] || *end != '\0' || errno == ERANGE)
		return ((void)set_env_var(env, "SHLVL", 1));
	if (lvl < 0)
		return ((void)set_env_var(env, "SHLVL", 0));
	next = lvl + 1;
	if (next >= 1000)
	{
		fprintf(stderr, "minishell: warning: shell level (%ld) ", next);
		fprintf(stderr, "too high, resetting to 1\n");
		return ((void)set_env_var(env, "SHLVL", 1));
	}
	set_env_var(env, "SHLVL", (int)next);
}

int	main(int argc, char **argv, char **envp)
{
	char	**env;
	int		status;

	(void)argc;
	(void)argv;
	env = dup_env(envp);
	if (!env)
		return (1);
	init_shlvl(&env);
	init_default_env(&env);
	signals_setup();
	if (!isatty(STDIN_FILENO))
		status = input_via_pipe(&env);
	else
		status = shell_loop(&env);
	cleanup_shell(env);
	return (status);
}
