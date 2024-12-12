#ifndef LEXER_H
#define LEXER_H

#include "minishell.h"

typedef enum e_token_type {
    T_WORD,
    T_PIPE,
    T_REDIR_IN,
    T_REDIR_OUT,
    T_APPEND,
    T_HEREDOC,
    T_SPACE,
    T_END
} t_token_type;

typedef struct s_token {
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

// lexer.c
t_token *lexer_tokenize(const char *line);
void free_token_list(t_token *tokens);

#endif
