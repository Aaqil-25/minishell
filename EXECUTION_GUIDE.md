# Minishell Execution Guide – From the Beginning

This guide walks you through implementing the **execution** part of minishell: what the subject requires, what you receive from the parser, and in what order to implement each piece.

---

## 1. What the subject requires (execution side)

| Requirement | Your job |
|-------------|----------|
| **Prompt** | Display a prompt when waiting for a new command (e.g. `minishell$ `). |
| **History** | Working history (use `readline()` + `add_history()`). |
| **Executable** | Search and launch the right executable: PATH, or relative/absolute path (`access`, `stat`, `execve`). |
| **One global for signals** | At most one global variable: store only the **signal number** (e.g. `int g_signal;`). No structs or data in global scope for this. |
| **Redirections** | `<` redirect input, `>` redirect output (truncate), `>>` append output, `<<` heredoc (read until line equals delimiter; no history). |
| **Pipes** | `|`: output of each command → input of the next via `pipe()` + `dup2()`. |
| **$?** | Expand to exit status of the **last foreground pipeline**. You must store this and expose it to the parser (e.g. global or context). |
| **Signals** | Ctrl-C: new prompt on new line. Ctrl-D: exit shell. Ctrl-\: do nothing. |
| **Builtins** | echo (-n), cd (relative/absolute path only), pwd (no options), export (no options), unset (no options), env (no options/args), exit (no options). |

---

## 2. What you receive from the parser

The parser gives you a **list of commands** (`t_command *`), one per pipeline segment (split by `|`).

For each command you have:

- **`args`** – `char **`, NULL-terminated: `args[0]` = command name, `args[1]` … = arguments.
- **`redirs`** – Linked list of `t_redir`: each has `type` (C_REDIR_IN, C_REDIR_OUT, C_REDIR_APPEND, C_REDIR_HEREDOC) and `filename` (for `<<` this is the **delimiter**).
- **`next`** – Next command in the pipeline (NULL if this is the last).

Empty pipeline (e.g. empty line) → parser can return NULL or a command with no args; you skip execution.

---

## 3. Execution flow (high level)

```
execute(t_command *cmds, t_shell *shell):
  1. If no commands or first command has no args[0]: return (set exit status 0 or as you like).
  2. For each command in the pipeline:
       - Resolve heredocs (<<): read stdin until a line equals delimiter, store in a temp file or pipe; that will be the input for this command.
       - Apply redirections (open files, dup2) – see below.
       - If builtin and single command: run builtin in process, set exit status, continue.
       - If external or pipeline: fork, in child apply redirs and either execve or run builtin, in parent collect exit status (for last process in pipeline).
  3. Save exit status of last command for $?.
```

Details for redirections, pipes, and builtins are in the sections below.

---

## 4. Suggested implementation order

Do them in this order so you can test step by step.

### Step 1: Main loop and prompt

- Use **readline** for the line and **add_history** when the line is non-empty.
- After parsing, if you have a command list, call your execution entry point (e.g. `execute(cmds, ...)`).
- Free the command list and the line when done.
- One global: e.g. `int g_signal_number;` – set in signal handler; only store the signal number (subject requirement).

### Step 2: Run a single command (no pipe, no redirections)

- Take the first (and only) command.
- If `args[0]` is NULL, nothing to run; return.
- **Builtin?** If so, call your builtin and set exit status; return.
- **Otherwise:** fork; in child call `execve(path, args, env)`; in parent `waitpid()` and set exit status from `WEXITSTATUS(status)`.
- For “path”: if `args[0]` contains `/`, use it as path (relative or absolute); else search in PATH (see Step 5).

### Step 3: Builtins (no redirections first)

Implement these so they work when there are no redirections. You can add redirection support later (dup2 before calling the builtin).

- **echo** – Print args from `args[1]` to end. If `args[1]` is `-n`, skip newline and print from `args[2]`. Use `write(1, ...)` or printf.
- **cd** – Only relative or absolute path. `chdir(args[1])`; on failure `perror` and set exit status (e.g. 1). No options.
- **pwd** – `getcwd(buffer, size)` and print it; no options.
- **exit** – No options. Exit the process with status 0 (or last exit status if you want bash-like behavior). If in main shell, exit the program; if in child (e.g. in a pipeline), exit the child.
- **env** – No options/args. Print your environment (the `char **` you maintain for export/unset).
- **export** – No options. For each `args[i]` (i≥1), add or update in your env copy (format `NAME=value`). No need to support `export` alone (no args) printing anything for mandatory.
- **unset** – No options. For each `args[i]` (i≥1), remove that variable from your env copy.

You need a **modifiable copy of the environment** (e.g. `char **env` or a list) because export/unset change it and you pass it to `execve()`.

### Step 4: Redirections (single command)

Before running one command (builtin or external):

- **Apply redirections in order.** For each `t_redir` in `redirs`:
  - **C_REDIR_IN** – `open(filename, O_RDONLY)`, then `dup2(fd, STDIN_FILENO)`, then `close(fd)`.
  - **C_REDIR_OUT** – `open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644)`, then `dup2(fd, STDOUT_FILENO)`, then `close(fd)`.
  - **C_REDIR_APPEND** – `open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644)`, then `dup2(fd, STDOUT_FILENO)`, then `close(fd)`.
  - **C_REDIR_HEREDOC** – See Step 5 below.

If any `open()` fails, perror and set exit status (e.g. 1); do not run the command. Apply redirs in the **child** for external commands; for builtins you can apply in the main process (or in a child if you prefer).

### Step 5: Heredoc (<<)

- Parser gives you the **delimiter** in `filename` of the HEREDOC redir.
- **In execution:** read from stdin (file descriptor 0) line by line (e.g. with `get_next_line(0)` or a simple loop) until you read a line that **equals** the delimiter (no trimming required by subject; bash trims; you can compare exact line).
- Subject: “it doesn’t have to update the history” – so you don’t add heredoc lines to readline history.
- You have two common options:
  - **Temp file:** write the collected lines to a temp file, then redirect stdin of the command from that file (`open()` + `dup2(..., STDIN_FILENO)`).
  - **Pipe:** write lines into a pipe and redirect stdin from the read end. (Slightly more work but no temp file.)
- Apply this redirection like the others (so the command’s stdin is the heredoc content).

### Step 6: Finding the executable (PATH / relative / absolute)

- If `args[0]` contains a `/`, treat it as a path: use `access(args[0], X_OK)` (or `stat`) to check; if ok, `execve(args[0], args, env)`.
- If no `/`, search in **PATH**: split `getenv("PATH")` by `:`; for each directory, try `directory/args[0]` with `access(..., X_OK)`; first success is the path to pass to `execve`.
- If not found, write an error (e.g. “minishell: cmd: command not found”), set exit status (e.g. 127), and do not exec.

### Step 7: Pipes (pipeline)

- For N commands you need N−1 pipes. Create all pipes first (e.g. array of `int pipe_fd[2]`).
- For command i (0-based):
  - If i > 0: redirect stdin from `pipe_fd[i-1][0]` (read end of previous pipe).
  - If i < N-1: redirect stdout to `pipe_fd[i][1]` (write end of current pipe).
  - Close all pipe fds in the child after dup2 (you don’t want the child to hold extra copies).
- Fork one process per command; each child applies its redirections and pipe ends, then exec (or run builtin). Parent waits for **all** children; **exit status of the pipeline** = exit status of the **last** command (last process). Save that for $?.

### Step 8: Signals

- **One global:** e.g. `volatile sig_atomic_t g_signal_number;` – in the handler only set `g_signal_number = signum`. Do not access other data structures in the handler.
- **Ctrl-C (SIGINT):** In handler set `g_signal_number = SIGINT`. In main loop after readline (or when you detect it), print a newline and show the prompt again; clear the global so you don’t react twice.
- **Ctrl-D (EOF):** readline returns NULL; exit the shell cleanly.
- **Ctrl-\ (SIGQUIT):** In handler set `g_signal_number` (or ignore); in interactive mode do nothing (don’t quit). So handler can be empty or just set global and do nothing else.
- For **child processes:** usually you want default behavior for SIGINT/SIGQUIT in the child (so the child dies on Ctrl-C). So set signals to default in the child after fork, or before execve.

---

## 5. Exit status and $?

- After running the last command of a **foreground** pipeline, set the exit status (e.g. from `waitpid` and `WEXITSTATUS`). Store it somewhere the parser can read for **$?** (e.g. a global `int g_exit_status` or a field in a context struct). Subject allows “at most one global” **for the signal**; storing exit status in another global or in a struct is fine.
- If the pipeline was aborted by a signal, you can set the exit status to 128 + signal number (bash-like).

---

## 6. Data you need to pass into execution

- **Command list** – `t_command *` from parser.
- **Environment** – A modifiable copy of the environment (for export/unset and for `execve(..., env)`). You can build this once at startup from `environ` or from `main(int argc, char **argv, char **envp)` and pass it (or a pointer to it) into `execute`.
- **Last exit status** – So the parser can expand $? (either a global or a pointer in a “shell context” struct).

---

## 7. Summary checklist (execution)

- [ ] Main loop: readline, add_history, parse, execute, free, prompt again.
- [ ] One global for signal number only; signal handlers only set it.
- [ ] Ctrl-C: new prompt on new line; Ctrl-D: exit; Ctrl-\: nothing.
- [ ] Run single command: builtin or execve (path from PATH or relative/absolute).
- [ ] Builtins: echo (-n), cd, pwd, export, unset, env, exit.
- [ ] Redirections: <, >, >>; apply in child (or before builtin).
- [ ] Heredoc: read until line == delimiter; feed as stdin (temp file or pipe).
- [ ] Pipes: N commands → N−1 pipes; connect stdout of i to stdin of i+1; wait for all; last exit status = $?.
- [ ] Save last foreground exit status for $?.

Once all of the above work, the execution part meets the mandatory requirements. Implement in the order above and test each step before moving on.

---

## 8. What’s already in the repo (scaffold)

- **minishell.h** – Declares `execute()`, all builtins, `g_signal_number`, `g_exit_status`.
- **execution.c** – Defines `g_signal_number` and `g_exit_status`, `execute()`, `run_single()`, `apply_redirections()`, `find_path()`, `is_builtin()`, `run_builtin()`. **Pipeline** (`run_pipeline`) currently just runs the first command; you implement full N-command pipes. **Heredoc** in `apply_redirections()` is a TODO.
- **builtins.c** – `echo`, `cd`, `pwd`, `env` are implemented; `export` and `unset` are stubs. `exit` calls `exit(g_exit_status)`.

You still need to: wire main loop to readline + execute; implement pipeline (pipes + fork each command); implement heredoc; implement export/unset (modify env copy); add signal handlers and respect `g_signal_number` in the main loop.

---

## 9. Integrating with main

1. **Environment copy** – In `main`, build a modifiable copy of `environ` (or use `envp` from `main(int ac, char **av, char **envp)`). You need a `char **` that you can realloc when adding/removing variables for export/unset. Pass it to `execute(cmds, env)`.

2. **Main loop (once parser returns commands)**:
   - `line = readline("minishell$ ");` if NULL (Ctrl-D), exit shell.
   - If `*line`, `add_history(line)`.
   - `tokens = lexer(line);` then `cmds = parser(&tokens);` (parser frees tokens).
   - If `cmds`: `execute(cmds, env);` then `free_commands(&cmds);`
   - Check `g_signal_number`: if SIGINT, print newline and prompt again; clear `g_signal_number`.
   - `free(line)` (readline allocates).

3. **Signals** – Before the loop, set up handlers for SIGINT and SIGQUIT. In the handler only: `g_signal_number = signum;`. In the loop, after readline or when deciding to prompt again, if `g_signal_number == SIGINT`, print newline and re-prompt; then set `g_signal_number = 0`. For SIGQUIT, do nothing (no quit). Child processes should get default signal behavior (set in child after fork, or don’t change in child).
