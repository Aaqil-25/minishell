
# Personal Notes

## Introduction

Minishell... finally minishell. Supposed to be the hardest project from here, here I am, self-assigned to do the parsing.


## Requirements

### Pt. 1 - 27/09/2025
- System shall have Libft and Get Next Line added and imported; - DONE
- System shall integrate already existing prompt_and_read function with another function that takes STDIN as a parameter IF STDIN is not empty; - DONE
- System shall split input argument as a list of strings; - IN PROGRESS
- System shall call a function that assigns each string as a token, assigns its token type, and return it all as a list of tokens;

### Pt. 2 - 08/10/2025
- System shall parse each token on respective command structures;
- System shall be divided in "find content of commands", "find relations with previous and next" and then "fill commands with proper inputs";

## List of Mallocs

It's important to keep track, so I can know if I am freeing it properly later on...

- array_of_words;

## Ongoing Notes

### Goals for Today

- Understand Lexing and Parsing concepts;
- Define project structure and header files;
- Lexer (Tokenization).

Ok ok ok... I should write requirements.

Something about... hmmm, let's think of what we already have.

Ohhh right, I need to add libft + get_next_line to use as a base.

Also I have a simple function where I can input STDIN and press enter to go to a next prompt line.

From this function I should have an if/else statement that only goes to next line when NOTHING is written.

If SOMETHING was written, it should call another function to handle the tokens -> divide it -> parse it -> call its execution. But these stuff are for later, now only integrate IF with another function where STDIN is the argument. 

Split argument as a list of strings.

Call a function that assigns each string as a token, assigns its token type, and return it all as a list of tokens.

-----------------

At some point I will need something similar to what I used to get_next_line to handle "infinite" sized inputs.

-----------------

I just tested and the splitting is being NULL terminated as expected.

-----------------

Ok, today is the day. Today I am finishing the basic functions of parsing.

Ehh ok, I finished it. Started planing the next steps as well.