/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:30:10 by ymazzett          #+#    #+#             */
/*   Updated: 2025/10/08 14:52:40 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "libft/libft.h"
# include "libft/gnl/get_next_line.h"
# define SIG_CTRL_C		2   // Ctrl-C (SIGINT)
# define SIG_CTRL_D		4   // Ctrl-D (EOF)
# define SIG_CTRL_BS	28  // Ctrl-\ (SIGQUIT)

typedef enum e_token_type
{
	WORD,		// Regular word: "ls", "-la", "filename.txt"
	PIPE,		// |
	REDIR_IN,	// <
	REDIR_OUT,	// >
	APPEND,		// >>
	HEREDOC		// <<
}	t_token_type;

typedef struct s_token
{
	char				*value;     // The string "ls", ">", etc.
	enum e_token_type	type;       // WORD, PIPE, REDIR_OUT, REDIR_IN, etc.
	struct s_token		*next;      // Pointer to next token in the list
}	t_token;

typedef enum e_redir_type
{
	C_REDIR_IN,		// < file
	C_REDIR_OUT,		// > file (truncate)
	C_REDIR_APPEND,	// >> file
	C_REDIR_HEREDOC	// << DELIMITER
}	t_redir_type;

typedef struct s_redir
{
	enum e_redir_type	type;
	char				*filename;	/* for << this is the delimiter */
	struct s_redir		*next;
}	t_redir;

typedef struct s_command
{
	char				**args;		/* array of strings, NULL-terminated */
	t_redir				*redirs;	/* list of all redirections (in order) */
	struct s_command	*next;		/* next command in pipeline (|) */
	struct s_command	*prev;
}	t_command;

/* Token / lexer */
t_token		*lexer(char *line);		/* line -> token list; NULL on error/unclosed quote */
void		free_tokens(t_token **head);

/* Parser: tokens -> command list */
t_command	*parser(t_token **head);	/* consumes/frees tokens; NULL on syntax error */
void		free_commands(t_command **head);

/* Utils */
void		free_array_of_words(char ***array_of_words);
size_t		arraylen(char **array);


#endif
