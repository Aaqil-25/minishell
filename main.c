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

// Determines the token type based on the string content
t_token_type	get_token_type(char *word)
{
	size_t	len;

	len = ft_strlen(word);
	if (!ft_strncmp(word, "|",len))
		return (PIPE);
	else if (!ft_strncmp(word, ">",len))
		return (REDIR_OUT);
	else if (!ft_strncmp(word, "<",len))
		return (REDIR_IN);
	else if (!ft_strncmp(word, ">>",len))
		return (APPEND);
	else if (!ft_strncmp(word, "<<",len))
		return (HEREDOC);
	else
		return (WORD);
}

// Creates a linked list of tokens from an array of words
t_token	*tokenize_all(char **array_of_words)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	int		i;

	if (!array_of_words || !array_of_words[0])
		return (NULL);
	head = create_token(array_of_words[0], get_token_type(array_of_words[0]));
	if (!head)
		return (NULL);
	current = head;
	i = 1;
	while (array_of_words[i])
	{
		new_token = create_token(array_of_words[i], get_token_type(array_of_words[i]));
		if (!new_token)
			return (head); // Return what we have so far
		current->next = new_token;
		current = new_token;
		i++;
	}
	return (head);
}

void	handle_input(char *input)
{
	char	**array_of_words;
	t_token	*tokens;

	array_of_words = ft_split(input, ' ');
	printf("DEBUG: Array of words [0]: %s\n", array_of_words[0]);

	tokens = tokenize_all(array_of_words);
	printf("DEBUG: Tokens [0]: %s\n", tokens->value);

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
