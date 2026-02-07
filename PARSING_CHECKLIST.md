# Minishell Parsing Checklist (vs Subject)

This document maps the **mandatory** subject requirements to the parsing layer and states what is done vs missing.

---

## 1. Subject requirements that affect parsing

| Requirement | Parsing responsibility | Status |
|-------------|------------------------|--------|
| **Quotes** | Single quote `'` prevents interpretation of meta-characters in the quoted part. | ❌ Not done |
| | Double quote `"` same, but `$` is still interpreted (variable expansion). | ❌ Not done |
| | Do **not** interpret unclosed quotes → treat as error or undefined; subject says “not interpret”. | ❌ Not done |
| | Do **not** interpret `\` (backslash) or `;` (semicolon). | ✅ N/A (don’t add them) |
| **Redirections** | `<` redirect input (one token + operand). | ⚠️ Token type exists; no real lexer |
| | `>` redirect output. | ⚠️ Same |
| | `>>` redirect output append. | ⚠️ Same |
| | `<<` heredoc: delimiter on same or next token; “read until line containing delimiter”. | ⚠️ Same |
| **Pipes** | `|` – output of each command → input of next. | ⚠️ Token type exists; no real lexer |
| **Variables** | `$` + sequence of characters → expand to env value. | ❌ Not done |
| | `$?` → exit status of last foreground pipeline. | ❌ Not done |

So the **parser** must:

1. **Lexer (tokenization)**  
   Split the line into tokens **respecting quotes** (so `echo "hello world"` is one word, and `|`, `<`, `>`, `>>`, `<<` are separate tokens even without spaces, e.g. `ls|cat`, `cat<file`).

2. **Quote handling**  
   - Group quoted segments into single tokens.  
   - Inside double quotes, allow `$` for expansion; inside single quotes, do not.  
   - Detect unclosed quotes and handle (e.g. error or “not interpret” as per subject).

3. **Variable expansion**  
   - Replace `$VAR` with `getenv("VAR")` (or similar).  
   - Replace `$?` with the last exit status (you’ll need a way to pass this in from execution).

4. **Quote removal**  
   After expansion, remove the quote characters from the final token strings that are passed to execution.

5. **Parser (tokens → commands)**  
   Build a list of commands (your `t_command`), each with:
   - `args`: command name + arguments (array of strings, NULL-terminated).
   - Redirections: for each command, attach list of `<`, `>`, `>>`, `<<` with their operand (file or heredoc delimiter).
   - Pipeline: link commands with `next` for `|`.

6. **Heredoc**  
   Parsing only needs to recognize `<<` and store the **delimiter** (next token). “Read until line containing delimiter” is **execution** (reading from stdin until that line).

---

## 2. What you already have

- **Structures (minishell.h)**  
  - `t_token` with types: `WORD`, `PIPE`, `REDIR_IN`, `REDIR_OUT`, `APPEND`, `HEREDOC`.  
  - `t_redir` (type + filename/delimiter).  
  - `t_command` (args, input, output, next).

- **Current “tokenization” (main.c)**  
  - Uses `ft_split(input, ' ')` then `tokenize_all()` to turn each “word” into a token.  
  - **Problems:**  
    - No quote handling: `echo "hello world"` becomes 3 tokens.  
    - No operator splitting: `ls|cat` or `ls>>out` stay one token.  
    - No variable expansion.  
    - No conversion from token list to `t_command` (no real parser).  
    - Token list is never freed (leak).

So: **data structures and token types are in place; the real lexer, expansion, and parser are missing.**

---

## 3. What must be implemented (parsing only)

### Phase A – Lexer (string → tokens)

- [ ] **Lexer** that:
  - Splits on **metacharacters** `|`, `<`, `>`, and the two-char operators `>>`, `<<`, **even when not surrounded by spaces** (e.g. `ls|cat`, `cat<in`).
  - Respects **quotes**: inside `'...'` or `"..."`, do not split on spaces or meta-characters; one quoted part = one token (or part of a token you merge).
  - Handles **unclosed quotes** (e.g. return error or a dedicated “unclosed quote” token and don’t interpret as subject says).
- [ ] **No** backslash or semicolon handling (subject says don’t interpret them).

### Phase B – Expansion (before or during token/command build)

- [ ] **Variable expansion**:  
  - `$NAME` → value from environment (e.g. `getenv`).  
  - `$?` → last exit status (e.g. global or argument from execution).
- [ ] Expansion only inside double quotes (and unquoted); **not** inside single quotes.

### Phase C – Quote removal

- [ ] After expansion, **remove** the quote characters from token values so execution sees clean strings (e.g. `"hello"` → `hello`).

### Phase D – Parser (tokens → command list)

- [ ] **Syntax checks**:  
  - Pipe at start/end: `| cmd` or `cmd |`.  
  - Consecutive operators: `cmd ||`, `cmd >`, `cmd <`, etc.  
  - Missing operand for redirection: `cmd >`, `cmd <`, `cmd <<`.  
  (Subject says “not interpret” problematic cases; clear errors help.)
- [ ] Build **t_command** list:  
  - Split token list by `PIPE` into segments; each segment = one command.  
  - For each segment: first WORD(s) → `args`; each `<`, `>`, `>>`, `<<` + next WORD → add to that command’s redirection list (filename or heredoc delimiter).
- [ ] **Heredoc**: store delimiter in the redirection (e.g. `t_redir` with type `C_REDIR_HEREDOC` and `filename` = delimiter). Reading until that line is **execution**, not parsing.

### Phase E – Data structures and memory

- [ ] **Redirections**: subject allows one input and one output per command; storing a **list** of redirections per command (e.g. `t_redir *next` in `t_redir`) is recommended so you can have e.g. `cmd < a > b` and apply in order (last wins in bash).
- [ ] **Free** token list and command list when done (no leaks in your code; readline leaks are allowed).

---

## 4. Suggested implementation order

1. **Lexer**  
   - One function that walks the string and emits tokens (WORD, PIPE, REDIR_*, etc.), respecting quotes and splitting `|`, `<`, `>`, `>>`, `<<` correctly.  
   - No expansion yet; just correct tokens.

2. **Parser**  
   - Consume token list: split by PIPE → for each segment, collect args and redirections into `t_command`.  
   - Handle syntax errors (leading/trailing pipe, missing redir operand, etc.).

3. **Variable expansion**  
   - Either expand when building tokens (after lexer) or when building args (during/after parser).  
   - Implement `$VAR` and `$?`; only in double-quoted or unquoted regions.

4. **Quote removal**  
   - Strip `'` and `"` from token values after expansion, before execution sees them.

5. **Heredoc**  
   - Parser: already done if you store `<< DELIMITER` in `t_redir`.  
   - Execution: read stdin line by line until a line equals delimiter, then pass that content as input to the command.

---

## 5. Quick reference: “Don’t do” (subject)

- Do **not** interpret `\` (backslash).  
- Do **not** interpret `;` (semicolon).  
- Do **not** interpret unclosed quotes (handle as error / don’t interpret).  
- Use **at most one global variable** for signals (e.g. signal number only); no globals for parser/execution data.

---

## 6. Summary table

| Component        | Required by subject | Current state |
|-----------------|---------------------|---------------|
| Token types     | Yes                 | ✅ In header  |
| Lexer (with quotes, operators) | Yes | ❌ Only ft_split by space |
| Variable expansion $VAR, $?    | Yes | ❌ Missing   |
| Quote removal   | Yes                 | ❌ Missing   |
| Redir/Pipe parsing into commands | Yes | ❌ Missing |
| t_command + redirs              | Yes | ✅ Struct exists; not built from input |
| Heredoc delimiter stored       | Yes | ✅ Struct; not filled by parser |
| Free tokens/commands           | Norm / no leaks     | ❌ Tokens not freed |

Once the lexer respects quotes and operators, expansion and quote removal are implemented, and the parser builds `t_command` (with redir list) from the token list, the **parsing part** will be complete for the mandatory minishell. Execution can then use the `t_command` list (and you’ll add heredoc reading and signal handling on the execution side).

---

## 7. Integration with main loop

When parsing is ready, the main loop should look like this (conceptually):

```c
line = readline("minishell$ ");   // or your prompt
if (!line) break;                 // Ctrl-D
if (*line) add_history(line);

t_token *tokens = lexer(line);
if (!tokens && had_unclosed_quote) { print error; free(line); continue; }

t_command *cmds = parser(&tokens);  // parser can free tokens
if (!cmds && had_syntax_error) { print error; free(line); continue; }

// execute(cmds);   // your execution layer
free_commands(&cmds);
free(line);
```

Use **readline** (allowed by subject) instead of `read()` for history and proper line editing. The subject allows readline’s own leaks; your code (lexer, parser, execution) must not leak.
