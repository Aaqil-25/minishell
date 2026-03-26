# Tokens do Minishell (além de WORD)

## 1. **PIPE** (`|`)

**O que faz:** Conecta a saída de um comando à entrada do próximo.

```bash
ls -la | grep txt | wc -l
# ls -la → saída vai para grep
# grep txt → saída vai para wc -l
```

**No parsing:** Indica que você deve criar um **novo comando** e encadeá-lo ao anterior.

---

## 2. **REDIR_OUT** (`>`)

**O que faz:** Redireciona a saída padrão (stdout) para um arquivo, **sobrescrevendo**.

```bash
echo "hello" > file.txt
# Cria/sobrescreve file.txt com "hello"
```

**No parsing:** Próximo token (WORD) é o **nome do arquivo de destino**.

---

## 3. **REDIR_APPEND** (`>>`)

**O que faz:** Redireciona stdout para um arquivo, **adicionando ao final**.

```bash
echo "world" >> file.txt
# Adiciona "world" ao final de file.txt
```

**No parsing:** Igual ao `>`, mas marca como "append mode".

---

## 4. **REDIR_IN** (`<`)

**O que faz:** Redireciona a entrada padrão (stdin) de um arquivo.

```bash
sort < unsorted.txt
# sort lê de unsorted.txt ao invés do teclado
```

**No parsing:** Próximo token é o **arquivo de origem**.

---

## 5. **HEREDOC** (`<<`)

**O que faz:** Cria entrada multi-linha até encontrar um delimitador.

```bash
cat << EOF
linha 1
linha 2
EOF
# cat recebe essas linhas como entrada
```

**No parsing:** Próximo token é o **delimitador** (EOF no exemplo).

---

## Estrutura Recomendada para Redirecionamentos

```c
typedef struct s_redir
{
    int             type;      // REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC
    char            *file;     // Nome do arquivo ou delimitador
    struct s_redir  *next;     // Próximo redirecionamento
}   t_redir;
```

## Exemplo Prático

```
Comando: "cat < input.txt | grep foo > output.txt"

Tokens:
[WORD:cat] [REDIR_IN] [WORD:input.txt] [PIPE] 
[WORD:grep] [WORD:foo] [REDIR_OUT] [WORD:output.txt]

Resultado:
Comando 1: args=["cat"], redirs=[{REDIR_IN, "input.txt"}]
           next→
Comando 2: args=["grep", "foo"], redirs=[{REDIR_OUT, "output.txt"}]
```

---

## Lógica no Parser

```
Para cada token:

Se for WORD:
  → Adicionar aos argumentos do comando atual

Se for REDIR_IN/REDIR_OUT/REDIR_APPEND/HEREDOC:
  1. Guardar o tipo do redirecionamento
  2. Avançar para próximo token (nome do arquivo/delimitador)
  3. Criar t_redir e adicionar à lista de redirs do comando
  4. Avançar token

Se for PIPE:
  1. Finalizar comando atual
  2. Criar novo comando
  3. Encadear: cmd_atual->next = novo_cmd
  4. novo_cmd->prev = cmd_atual
  5. Atualizar cmd_atual = novo_cmd
  6. Avançar token
```

---

## Fluxo Completo do Parser

```
1. Inicializar primeira comando
2. Loop pelos tokens:
   - Coletar WORDs como argumentos
   - Processar redirecionamentos
   - Criar novo comando ao encontrar PIPE
3. Retornar o primeiro comando da lista encadeada
```

---

## Dicas de Implementação

- **Sempre validar:** Se há próximo token após operadores
- **Ordem importa:** `ls > out.txt | grep` é diferente de `ls | grep > out.txt`
- **Múltiplos redirs:** `cmd < in.txt > out.txt >> log.txt` é válido
- **Heredoc especial:** Precisa ler entrada do usuário até o delimitador

# For Pipes
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int pipefd[2]; // Array to hold two file descriptors
    pid_t pid1, pid2;
    
    // Step 1: Create the pipe
    // pipefd[0] - read end of the pipe
    // pipefd[1] - write end of the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    // Step 2: Fork first child process (for command1)
    pid1 = fork();
    
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid1 == 0) { // First child process
        // Step 3: Redirect stdout to pipe's write end
        // Close the read end - this child doesn't need it
        close(pipefd[0]);
        
        // Redirect stdout to pipe's write end
        // dup2 makes pipefd[1] a copy of stdout (file descriptor 1)
        dup2(pipefd[1], STDOUT_FILENO);
        
        // Close the original write end - no longer needed after dup2
        close(pipefd[1]);
        
        // Step 4: Execute first command
        // Example: "ls -l"
        execlp("ls", "ls", "-l", NULL);
        
        // If execlp fails
        perror("execlp ls");
        exit(EXIT_FAILURE);
    }
    
    // Step 2: Fork second child process (for command2)
    pid2 = fork();
    
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid2 == 0) { // Second child process
        // Step 3: Redirect stdin from pipe's read end
        // Close the write end - this child doesn't need it
        close(pipefd[1]);
        
        // Redirect stdin from pipe's read end
        // dup2 makes pipefd[0] a copy of stdin (file descriptor 0)
        dup2(pipefd[0], STDIN_FILENO);
        
        // Close the original read end - no longer needed after dup2
        close(pipefd[0]);
        
        // Step 4: Execute second command
        // Example: "sort -k5n" (sort by 5th column numerically)
        execlp("sort", "sort", "-k5n", NULL);
        
        // If execlp fails
        perror("execlp sort");
        exit(EXIT_FAILURE);
    }
    
    // Parent process
    // Close both pipe ends - parent doesn't use the pipe
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Wait for both child processes to complete
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    printf("Pipe emulation completed successfully!\n");
    
    return 0;
}
```

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define CHECK(condition, message) \
    if (!(condition)) { \
        fprintf(stderr, "Error: %s - %s\n", message, strerror(errno)); \
        exit(EXIT_FAILURE); \
    }

int main() {
    int pipefd[2];
    pid_t pid1, pid2;
    int status;
    
    // Step 1: Create pipe
    CHECK(pipe(pipefd) != -1, "Failed to create pipe");
    
    // Step 2: Fork first child
    pid1 = fork();
    CHECK(pid1 != -1, "Failed to fork first child");
    
    if (pid1 == 0) { // First child
        // Close unused read end
        close(pipefd[0]);
        
        // Redirect stdout to pipe
        CHECK(dup2(pipefd[1], STDOUT_FILENO) != -1, 
              "Failed to redirect stdout");
        
        close(pipefd[1]); // Close original write end
        
        // Execute command
        execlp("ls", "ls", "-l", NULL);
        
        // If we get here, exec failed
        fprintf(stderr, "Failed to execute ls: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    // Step 2: Fork second child
    pid2 = fork();
    CHECK(pid2 != -1, "Failed to fork second child");
    
    if (pid2 == 0) { // Second child
        // Close unused write end
        close(pipefd[1]);
        
        // Redirect stdin from pipe
        CHECK(dup2(pipefd[0], STDIN_FILENO) != -1, 
              "Failed to redirect stdin");
        
        close(pipefd[0]); // Close original read end
        
        // Execute command
        execlp("sort", "sort", "-k5n", NULL);
        
        // If we get here, exec failed
        fprintf(stderr, "Failed to execute sort: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Wait for children and check their exit status
    waitpid(pid1, &status, 0);
    if (WIFEXITED(status)) {
        printf("First child (ls) exited with status: %d\n", 
               WEXITSTATUS(status));
    }
    
    waitpid(pid2, &status, 0);
    if (WIFEXITED(status)) {
        printf("Second child (sort) exited with status: %d\n", 
               WEXITSTATUS(status));
    }
    
    return 0;
}
```

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMANDS 10
#define MAX_ARGS 10

// Structure to hold command information
typedef struct {
    char *cmd; // Command name (e.g., "ls")
    char *args[MAX_ARGS]; // Arguments (e.g., ["ls", "-l", NULL])
} command_t;

void execute_pipeline(command_t commands[], int num_commands) {
    int i;
    int prev_pipe[2] = {-1, -1}; // Previous pipe's read end
    pid_t pids[MAX_COMMANDS];
    
    for (i = 0; i < num_commands; i++) {
        int current_pipe[2];
        
        // Create pipe for all except last command
        if (i < num_commands - 1) {
            if (pipe(current_pipe) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        
        // Fork child process
        pids[i] = fork();
        
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        
        if (pids[i] == 0) { // Child process
            // Set up input from previous pipe (if not first command)
            if (i > 0) {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
                close(prev_pipe[1]); // Previous pipe write end already closed in parent
            }
            
            // Set up output to next pipe (if not last command)
            if (i < num_commands - 1) {
                close(current_pipe[0]); // Child doesn't need read end
                dup2(current_pipe[1], STDOUT_FILENO);
                close(current_pipe[1]);
            }
            
            // Close all pipe file descriptors in child
            // (prev_pipe already handled, current_pipe handled above)
            
            // Execute the command
            execvp(commands[i].cmd, commands[i].args);
            
            // If we get here, exec failed
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        
        // Parent process
        // Close previous pipe's read end (we're done with it)
        if (i > 0) {
            close(prev_pipe[0]);
            close(prev_pipe[1]);
        }
        
        // Save current pipe for next iteration
        if (i < num_commands - 1) {
            prev_pipe[0] = current_pipe[0];
            prev_pipe[1] = current_pipe[1];
        }
    }
    
    // Parent closes last pipe if it exists
    if (num_commands > 1) {
        close(prev_pipe[0]);
        close(prev_pipe[1]);
    }
    
    // Wait for all children
    for (i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }
}

int main() {
    // Example: ls -l | grep ".c" | sort -k5n | head -5
    
    command_t commands[4];
    
    // Command 1: ls -l
    commands[0].cmd = "ls";
    commands[0].args[0] = "ls";
    commands[0].args[1] = "-l";
    commands[0].args[2] = NULL;
    
    // Command 2: grep ".c"
    commands[1].cmd = "grep";
    commands[1].args[0] = "grep";
    commands[1].args[1] = "\\.c";
    commands[1].args[2] = NULL;
    
    // Command 3: sort -k5n
    commands[2].cmd = "sort";
    commands[2].args[0] = "sort";
    commands[2].args[1] = "-k5n";
    commands[2].args[2] = NULL;
    
    // Command 4: head -5
    commands[3].cmd = "head";
    commands[3].args[0] = "head";
    commands[3].args[1] = "-5";
    commands[3].args[2] = NULL;
    
    printf("Executing: ls -l | grep \".c\" | sort -k5n | head -5\n\n");
    execute_pipeline(commands, 4);
    
    return 0;
}
```
