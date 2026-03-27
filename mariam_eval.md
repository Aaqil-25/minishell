# Mock Evaluation

## Recommended Structure (Using Only Existing Files)

This is a reorganization plan with only files that already exist in the project.

```text
aaqil\_minishell/
├── include/
│   ├── minishell.h
│   └── exec\_internal.h
├── src/
│   ├── main/
│   │   ├── main.c
│   │   ├── loop.c
│   │   └── term.c
│   ├── parsing/
│   │   ├── token.c
│   │   ├── tokenize.c
│   │   ├── quotes\_token.c
│   │   ├── quotes\_split.c
│   │   ├── parsing.c
│   │   ├── parse\_cmd\_init.c
│   │   ├── parse\_command\_create.c
│   │   ├── parse\_redir.c
│   │   ├── parse\_env\_args.c
│   │   ├── parse\_env\_expand.c
│   │   ├── parse\_env\_part.c
│   │   └── parse\_exit\_status.c
│   ├── execution/
│   │   ├── execution.c
│   │   ├── execution\_ext.c
│   │   ├── execution\_builtin.c
│   │   ├── exec\_builtin.c
│   │   ├── exec\_env.c
│   │   ├── exec\_path.c
│   │   ├── exec\_redir.c
│   │   ├── pipeline.c
│   │   ├── pipeline2.c
│   │   └── input\_pipe.c
│   ├── builtins/
│   │   ├── builtins.c
│   │   ├── builtins2.c
│   │   ├── builtin\_cd.c
│   │   ├── builtin\_echo.c
│   │   ├── builtin\_env.c
│   │   ├── builtin\_exit.c
│   │   ├── builtin\_export.c
│   │   ├── builtin\_pwd.c
│   │   └── builtin\_unset.c
│   ├── signals/
│   │   └── signals.c
│   └── utils/
│       └── utils.c
├── docs/
│   ├── README.md
│   ├── MOCK\_EVAL.md
│   ├── EXECUTION\_GUIDE.md
│   ├── PERSONAL\_NOTES.md
│   └── cola.md
├── tests/
│   ├── test
│   └── test.test
├── libft/
├── bin/
├── Makefile
└── minishell
```

## Why This Split

* `include/`: all headers in one place.
* `src/parsing/`: tokenization, quote handling, and parser/expander pieces.
* `src/execution/`: command execution, path/env execution helpers, redirections, and pipelines.
* `src/builtins/`: builtin command implementations and dispatch files.
* `docs/`: markdown documentation files grouped together. "they can be deleted when submitting"
* `tests/`: current test artifacts grouped together. "they can be deleted when ssubmitting"

## Test cases

## Built-in

* echo  
`-nnnnnn` or many `-n` flags after each other should be treated as `-n`.  
Example:  
`echo -nnnnnnnn fs`  
`echo -n -n`
* export  
When you run `export` alone, it should display all environment variables in a specific format:

  * starts with `declare -x`
  * sorted
  * values are surrounded with double quotes (`""`)

  Also important: when you run `export fds` (without `=`), `export` shows it, but `env` does not.

  Example:  
`bash:$ export fds`  
`bash:$ export | grep fds`  
`declare -x fds`  
`bash:$ env | grep fds`

  If you add `=`, it is shown in both:  
`bash:$ export go=`  
`bash:$ export | grep "go"`  
`declare -x go=""`  
`bash:$ env | grep go`  
`go=`

  Note: `export \_TEST=100` did not show in `env` in one test run.

* unset  
Does not work at all.  
I checked its code; it is not implemented yet.

  ## expansions

* Example:  
`echo $USER$12USER$USER=4$USER12`  
`echo $USER $123456789USER $USER123456789`  
`echo $USER $USER9999 $USER8888 $USER7777`

  What should happen: when a number comes right after `$`, skip that part and keep going.

  ## error code

* Example:  
`| test`  
`echo > <`  
`echo | |`

  The error message is correct, but the exit code should be `2` instead of `0`.

* Example:  
`fhdggh`  
Exit code for "command not found" should be `127`.
* Most of the errors do not give the correct exit codes, so try to go through them all and compare with bash; that will help.

  ## Environment variables

* SHLVL:  
It is an environment variable that shows shell nesting level.  
For example, if you run minishell inside minishell, `SHLVL` should increase by 1.  
If it is `2`, it should become `3`.

  `SHLVL` has many edge cases. Try changing its value, then open a new shell and check how it changes.

  Examples:  
`export SHLVL=0`  
then run:  
`./minishell`  
then check the current value of `SHLVL`.

  `export SHLVL=546546451541`  
then run:  
`./minishell`  
then check the current value of `SHLVL`.

  You can try more cases too.

  # env -i case

* If you run minishell this way:  
`env -i ./minishell`

  Bash normally creates default environment variables.  
In your case it may be empty.  
Decide if you want to handle it (me and Ana did).

  # error message

* When command not found, it should be like:

  `unfound command: command not found`

  In your case it is:

  `minishell: command not found`

* The syntax errors have some edge cases; they do not always take the first character.

  Example: `< |`

  * Correct error message: `bash: syntax error near unexpected token \\`|'`
  * Your error message: `minishell: syntax error near redirection`
* Example: `<<`

  * Correct error message: `bash: syntax error near unexpected token `newline'`
  * Your error message: `minishell: syntax error near redirection`

  # redirection

* here doc

  * Personal opinion: having the same display for the here-doc is confusing.  
When I am inside minishell you can have `minishell >` on each line instead of `>` for both the here-doc and the minishell.
  * Expansions should work in most cases inside the here-doc.  
In your case they do not work at all.

  Example:  
`cat << EOF`  
`$PWD`  
`EOF`

  In this case the here-doc will display the value of `PWD`.  
So the result will be something like:  
`/home/user`

  A weird thing you should consider: when the delimiter starts with `"`, for some reason expansion does not work.

  Example:  
`cat << "EOF"`  
`$PWD`  
`EOF`

  This will result in:  
`$PWD`

  (no expansion)

