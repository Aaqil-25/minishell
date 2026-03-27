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

static int	env_key_match(char *entry, char *key)
{
	size_t	len;

	len = ft_strlen(key);
	return (ft_strncmp(entry, key, len) == 0 && entry[len] == '=');
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
	char	*num;
	char	*tmp;
	char	*entry;
	char	**new_env;
	int		i;

	num = ft_itoa(value);
	if (!num)
		return (0);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (free(num), 0);
	entry = ft_strjoin(tmp, num);
	free(num);
	free(tmp);
	if (!entry)
		return (0);
	i = 0;
	while ((*env)[i])
	{
		if (env_key_match((*env)[i], key))
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
	long	lvl;

	val = exec_get_env_value(*env, "SHLVL");
	if (!val)
		set_env_var(env, "SHLVL", 1);
	else
	{
		lvl = ft_atoi(val);
		if (lvl < 0)
			set_env_var(env, "SHLVL", 0);
		else if (lvl >= 1000)
			set_env_var(env, "SHLVL", 1);
		else
			set_env_var(env, "SHLVL", (int)(lvl + 1));
	}
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
	signals_setup();
	if (!isatty(STDIN_FILENO))
		status = input_via_pipe(&env);
	else
		status = shell_loop(&env);
	free_array_of_words(&env);
	return (status);
}
