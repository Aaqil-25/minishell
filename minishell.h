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
	REDIR_IN = 0,
	REDIR_OUT = 1,
	APPEND = 2,
	HEREDOC = 3,
	WORD = 4,
	PIPE = 5
}	t_token_type;

typedef struct s_token
{
	char				*value;
	enum e_token_type	type;
	struct s_token		*next;
}	t_token;

typedef enum e_redir_type
{
	C_REDIR_IN = 0,
	C_REDIR_OUT = 1,
	C_REDIR_APPEND = 2,
	C_REDIR_HEREDOC = 3
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

/* Signal: subject allows one global (signal number only). */
static volatile sig_atomic_t	g_signal = 0;

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
char		**append_to_array(char **array, char *new_str);

/* Environment utilities */
char		**env_dup(char **envp);
int			env_find_index(char **env, const char *name);
int			env_unset_entry(char ***env, const char *name);

/* Execution returns last exit status (store it in your shell state). */
int			execute(t_command *cmds, char ***env);

/* Builtins: return exit status. env only for export/unset/env. */
int			builtin_echo(char **args);
int			builtin_cd(char **args);
int			builtin_pwd(char **args);
int			builtin_export(char **args, char ***env);
int			builtin_unset(char **args, char ***env);
int			builtin_env(char **args, char **env);
int			builtin_exit(char **args, int last_status);

/* --------------------tmp functions------------------------------ */
/* Interactive shell helpers */
void		signals_setup(void);
void		term_apply(int prompt_mode);
int			shell_loop(char ***env);

#endif
