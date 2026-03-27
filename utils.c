/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:58:34 by ymazzett          #+#    #+#             */
/*   Updated: 2025/10/08 14:55:17 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array_of_words(char ***array_of_words)
{
	int	i;

	if (!array_of_words || !*array_of_words)
		return ;
	i = 0;
	while ((*array_of_words)[i])
	{
		free((*array_of_words)[i]);
		i++;
	}
	free(*array_of_words);
	*array_of_words = NULL;
}

size_t	arraylen(char **array)
{
	size_t	len;

	len = 0;
	if (!array)
		return (0);
	while (array[len] != NULL)
		len++;
	return (len);
}

static char	**create_new_array(char *new_str)
{
	char	**new_array;

	new_array = malloc(2 * sizeof(char *));
	if (!new_array)
		return (NULL);
	new_array[0] = ft_strdup(new_str);
	new_array[1] = NULL;
	return (new_array);
}

static void	copy_array(char **new_array, char **array, char *new_str)
{
	size_t	i;

	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		i++;
	}
	new_array[i] = ft_strdup(new_str);
	new_array[i + 1] = NULL;
}

char	**append_to_array(char **array, char *new_str)
{
	char	**new_array;
	size_t	i;

	if (!array)
		return (create_new_array(new_str));
	i = 0;
	while (array[i])
		i++;
	new_array = malloc((i + 2) * sizeof(char *));
	if (!new_array)
		return (NULL);
	copy_array(new_array, array, new_str);
	free_array_of_words(&array);
	return (new_array);
}
