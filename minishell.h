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
# include <sys/wait.h>
# include <fcntl.h>
# include <limits.h>

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
extern volatile sig_atomic_t	g_signal;

/* Token / lexer */
t_token		*lexer(char *line);
int			is_separator(char c);
int			is_quote(char c);
int			quotes_token_len(const char *s, int i);
char		**wd_n_quotes_split(char const *s);
void		free_tokens(t_token **head);
t_token		*tokenize_all(char **array_of_words);

/* Parser: tokens -> command list */
int			parse_word_token(t_command *cmd, t_token **current_token);
int			parse_command_tokens(t_command *cmd,\
	t_token **current_token, t_redir **last_redir);
t_command	*create_command(t_token **current_token);
int			is_redirection(t_token_type type);
t_redir		*add_redirection(t_redir *redir, t_redir *last_redir,\
	t_token *op_token);
t_redir		*parse_redirection(t_redir *redir,\
	t_redir **last_redir, t_token **current_token);
t_command	*init_command(void);
void		append_command(t_command **cmds, t_command *new_cmd);
t_command	*parser(t_token **head);
void		free_commands(t_command **head);
char		*env_append_part(char *base, const char *part, size_t part_len);
char		*env_substitute(const char *s, size_t *i, char **env);
char		*parse_env_in_string(char **str, char **env);
void		parse_env_variable(t_command *cmds, char **env);
int			last_exit_status(int new_status);

/* Utils */
void		free_array_of_words(char ***array_of_words);
size_t		arraylen(char **array);
char		**append_to_array(char **array, char *new_str);
int			prompt_and_read(char **env);
int			input_via_pipe(char **env);
char		*exec_get_env_value(char **env, char *name);
void		handle_input(char *input, char **env);

/* Execution returns last exit status (store it in your shell state). */
int			execute(t_command *cmds, char **env);

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
int			shell_loop(char **env);

#endif
