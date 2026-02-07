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
# include <signal.h>
# include "libft/libft.h"
# include "libft/gnl/get_next_line.h"
# include <readline/readline.h>
# include <readline/history.h>
# define PROMPT	"> "

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}	t_token_type;

typedef struct s_token
{
	char				*value;
	enum e_token_type	type;
	struct s_token		*next;
}	t_token;

typedef enum e_redir_type
{
	C_REDIR_IN,
	C_REDIR_OUT,
	C_REDIR_APPEND,
	C_REDIR_HEREDOC
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

/* Signal: at most one global, subject-mandated */
extern volatile sig_atomic_t	g_signal;

/* Token / lexer */
t_token		*lexer(char *line);
void		free_tokens(t_token **head);
t_token		*tokenize_all(char **array_of_words);

/* Parser: tokens -> command list */
t_command	*parser(t_token **head);
void		free_commands(t_command **head);

/* Utils */
void		free_array_of_words(char ***array_of_words);
size_t		arraylen(char **array);


#endif
