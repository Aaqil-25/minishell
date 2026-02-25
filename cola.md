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