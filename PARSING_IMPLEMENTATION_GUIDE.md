# Parsing Implementation Guide – Step by Step

Use this together with **PARSING_CHECKLIST.md**. Order of implementation below matches the checklist.

---

## Step 1: Lexer (string → tokens)

The lexer walks the line character by character and produces a linked list of `t_token`.

### Rules

- **Metacharacters** (outside quotes): `|`, `<`, `>`. When you see `>`, peek next; if `>`, token is `>>`. When you see `<`, peek next; if `<`, token is `<<`.
- **Whitespace**: skip; separates tokens (e.g. `cmd` and `|` can be separated by space).
- **Single-quoted string** `'...'`: from `'` to the next `'`, everything is literal (including `$`). One token = contents (or full `'...'` and strip quotes later).
- **Double-quoted string** `"..."`: from `"` to the next `"`, everything is literal **except** `$` (expansion done in Step 3). One token = contents.
- **Unclosed quote**: if you reach end of line inside `'` or `"`, return error (e.g. NULL) and do not interpret (per subject).
- **Word**: any other run of non-whitespace, non-meta characters. Stop at space, `|`, `<`, `>`, or start of quote.

### Algorithm (high level)

```
lexer(line):
  tokens = empty list
  i = 0
  while i < len(line):
    skip whitespace
    if end of line: break
    if line[i] == '|':  add token PIPE "|"; i++
    else if line[i] == '<':
      if line[i+1] == '<': add token HEREDOC "<<"; i += 2
      else:                add token REDIR_IN "<"; i++
    else if line[i] == '>':
      if line[i+1] == '>': add token APPEND ">>"; i += 2
      else:                add token REDIR_OUT ">"; i++
    else if line[i] == '\'':
      start = i+1
      find next '\'' (or end); if end → unclosed quote, return NULL
      add WORD token = substring(start, end); i = end+1
    else if line[i] == '"':
      start = i+1
      find next '"' (or end); if end → unclosed quote, return NULL
      add WORD token = substring(start, end); i = end+1
    else:
      start = i
      while i < len and not (space or | or < or > or quote): i++
      add WORD token = substring(start, i)
  return tokens
```

Important: **Do not** split on space inside quotes. For `echo "hello world"`, the single WORD token value can be `hello world` (quotes stripped in a later step) or `"hello world"` (strip in quote-removal step).

### Edge cases

- `ls|cat` → WORD "ls", PIPE, WORD "cat".
- `cat<file` → WORD "cat", REDIR_IN, WORD "file".
- `echo 'don'"'"'t'` → subject doesn’t require this; you can support it later. For mandatory, focus on simple quoted strings.
- Empty line or only spaces → return empty token list (parser can treat as no-op).

---

## Step 2: Parser (tokens → command list)

- Split the token list by **PIPE**: each segment is one command.
- For each segment:
  - Collect **WORD** tokens into `args` (first = command name, rest = arguments).
  - For **REDIR_IN**, **REDIR_OUT**, **APPEND**, **HEREDOC**: next token must be WORD (filename or delimiter); add a `t_redir` to this command’s `redirs` list; then skip that WORD (it’s the operand, not an arg).

### Syntax errors to catch

- **PIPE** at start or end: `| cmd` or `cmd |`.
- **PIPE** with nothing between: `cmd || other`.
- Redirection operator with no following WORD: `cmd >`, `cmd <`, `cmd <<`.
- You can also treat consecutive operators as error: e.g. `cmd > > file`.

On error: free the token list (if not already consumed), free any commands built so far, return NULL.

### Building t_command

- For each pipe segment, create one `t_command`.
  - `args`: array of strings (WORD values), last element NULL.
  - `redirs`: linked list of `t_redir`; each has type (C_REDIR_IN, C_REDIR_OUT, C_REDIR_APPEND, C_REDIR_HEREDOC) and `filename` = the next WORD’s value (for `<<`, this is the delimiter).
- Link commands with `next` (and optionally `prev`).

---

## Step 3: Variable expansion

- Apply **only** in double-quoted strings and in unquoted text. **Never** expand inside single quotes.
- **$?** → replace with string value of last exit status (e.g. from a global or a parameter your execution layer sets).
- **$VAR** → `VAR` = sequence of alphanumeric/underscore; replace with `getenv("VAR")`; if unset, replace with empty string (bash-like).
- You can do expansion either:
  - **In the lexer**: when building a WORD that came from double quotes or is unquoted, scan for `$` and expand before adding the token, or
  - **After lexer, before parser**: walk each WORD token and expand in place, or
  - **After parser**: walk each `args` and each redir `filename` and expand.

Easiest: after lexer (or when building WORD tokens), for each segment that is “expandable” (unquoted or inside `"`), replace `$VAR` and `$?` in the string and store the result in the token value.

---

## Step 4: Quote removal

- After expansion, remove the quote characters from token values so execution sees plain strings.
- If you stored quoted content without the surrounding `'` or `"`, this step may be a no-op for those. If you stored full `"hello"`, then strip the `"` so result is `hello`.

---

## Step 5: Memory and helpers

- **free_tokens(t_token **head)**: walk list, free `value`, free node, set *head = NULL.
- **free_commands(t_command **head)**: for each command, free `args` (each string + array), free `redirs` list (each `filename` + node), free node; set *head = NULL.
- Parser can **consume** the token list (free tokens as it builds commands) so you don’t need to return tokens; just return command list or NULL.

---

## Suggested file split

- **token.c / lexer.c**: `lexer()`, `free_tokens()`, and any small helpers (e.g. “get next token from string”).
- **parser.c**: `parser()`, `free_commands()`, and helpers to build one command from a segment of tokens.
- **expansion.c** (optional): `expand_variables(char *s, int in_double_quotes, int last_exit_status)` used by lexer or right after.

---

## Testing the parser (without execution)

- `lexer("echo hello")` → one WORD "echo", one WORD "hello".
- `lexer("echo \"hello world\"")` → one WORD "echo", one WORD "hello world" (or with quotes to remove later).
- `lexer("cat < in | wc -l")` → WORD "cat", REDIR_IN, WORD "in", PIPE, WORD "wc", WORD "-l".
- `parser(lexer("echo a | cat"))` → two commands: args ["echo","a"] and args ["cat"]; both redirs = NULL.
- Unclosed quote: `lexer("echo 'hello")` → NULL (or error).

Once these behave correctly, you can plug the command list into the execution part (your part) and add builtins, pipes, and redirections there.
