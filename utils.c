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
