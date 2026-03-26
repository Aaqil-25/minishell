/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:48:44 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:48:46 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_internal.h"

char	*exec_get_env_value(char **env, char *name)
{
	size_t	len;
	int		i;

	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (&env[i][len + 1]);
		i++;
	}
	return (NULL);
}
