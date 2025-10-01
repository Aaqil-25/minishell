/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:58:34 by ymazzett          #+#    #+#             */
/*   Updated: 2025/10/01 14:59:46 by codespace        ###   ########.fr       */
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

size_t arraylen(char **array)
{
	size_t len = 0;

	if (!array)
		return 0;
	while (array[len] != NULL)
		len++;
	return len;
}

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

// LOGICS FOR TOKENIZING
// - If the word is "|", create a PIPE token
// - If the word is ">", create a REDIR_OUT token
// - If the word is "<", create a REDIR_IN token
// - If the word is ">>", create an APPEND token
// - If the word is "<<", create a HEREDOC token
// - Otherwise, create a WORD token
t_token	*tokenize(char **array_of_words)
{
	int	i;

	if (!array_of_words || !*array_of_words)
		return (NULL);
	i = 0;
	while (array_of_words[i])
	{
		if (!ft_strcmp(array_of_words[i], "|"))
			return (create_token(array_of_words[i], PIPE));
		else if (!ft_strcmp(array_of_words[i], ">"))
			return (create_token(array_of_words[i], REDIR_OUT));
		else if (!ft_strcmp(array_of_words[i], "<"))
			return (create_token(array_of_words[i], REDIR_IN));
		else if (!ft_strcmp(array_of_words[i], ">>"))
			return (create_token(array_of_words[i], APPEND));
		else if (!ft_strcmp(array_of_words[i], "<<"))
			return (create_token(array_of_words[i], HEREDOC));
		else
			return (create_token(array_of_words[i], WORD));
		i++;
	}
	return (NULL);
} // The problem with this function is that it only creates one token and returns. It should create a linked list of tokens and return the head of the list.

void	handle_input(char *input)
{
	char	**array_of_words;

	array_of_words = ft_split(input, ' ');
	printf("DEBUG: Array of words [0]: %s\n", array_of_words[0]);

	free_array_of_words(&array_of_words);
}

int	prompt_and_read(void)
{
	char	buffer[1024]; // ALSO IS CAUSING A LEAK
	int		bytes_read;

	write(1, "> ", 2);
	bytes_read = read(0, buffer, 1023); // THIS WILL CAUSE AN ERROR FOR BIGGER INPUTS
	if (bytes_read < 0)
	{
		write(2, "Read error\n", 12);
		return (1);
	}
	else if (bytes_read > 1)
	{
		buffer[bytes_read] = '\0';
		printf("You entered: %s", buffer);
		handle_input(buffer);
	}
	return (0);
}

int	main(void)
{
	int	error;

	error = 0;
	while (!error)
		error = prompt_and_read();
	if (error == 1)
		return (1);
	return (0);
}
