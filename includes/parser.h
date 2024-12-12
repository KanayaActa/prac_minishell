#ifndef PARSER_H
#define PARSER_H

#include "minishell.h"

// For storing redirections
typedef enum e_redir_type {
    R_INPUT,
    R_OUTPUT,
    R_APPEND,
    R_HEREDOC
} t_redir_type;

typedef struct s_redir {
    t_redir_type type;
    char *filename;
    struct s_redir *next;
} t_redir;

typedef struct s_command {
    char **argv;
    t_redir *redir;
    struct s_command *next;
} t_command;

// parser.c
t_command *parse_line(t_shell *shell, const char *line);
void free_command_list(t_command *cmd);

// helper
t_redir *redir_new(t_redir_type type, char *file);
void    redir_add_back(t_redir **lst, t_redir *new);

// parse tokens from lexer
// The parser will create a linked list of commands separated by '|'
#endif
