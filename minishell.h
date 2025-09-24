/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:30:10 by ymazzett          #+#    #+#             */
/*   Updated: 2025/09/24 14:36:16 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# define SIG_CTRL_C    2   // Ctrl-C (SIGINT)
# define SIG_CTRL_D    4   // Ctrl-D (EOF)
# define SIG_CTRL_BS   28  // Ctrl-\ (SIGQUIT)

typedef enum e_token_type
{
	T_WORD,			// Regular word: "ls", "-la", "filename.txt"
	T_PIPE,			// |
	T_REDIR_IN,		// <
	T_REDIR_OUT,	// >
	T_APPEND,		// >>
	T_HEREDOC		// <<
}	t_token_type;

typedef struct s_token
{
    char            *value;     // The string "ls", ">", etc.
    enum e_token_type type;       // WORD, PIPE, REDIR_OUT, REDIR_IN, etc.
    struct s_token  *next;      // Pointer to next token in the list
}   t_token;

typedef enum e_redir_type
{
	REDIR_IN,		// < file
	REDIR_OUT,		// > file (truncate)
	REDIR_APPEND,	// >> file
	REDIR_HEREDOC	// << DELIMITER
}	t_redir_type;

typedef struct s_redir
{
    enum e_redir_type  type;
    char               *filename;
}   t_redir;

typedef struct s_command
{
    char            **args;         // Array of strings, ending with NULL
    t_redir         *input;         // Input redirection (< file)
    t_redir         *output;        // Output redirection (>, >>)
    struct s_command *next;         // Next command in the pipeline (if there's a pipe |)
    struct s_command *prev;         // Sometimes useful
}   t_command;

#endif
