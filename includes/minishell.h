// include/minishell.h
#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <dirent.h>
#include <termios.h>
#include "libft.h" // Ensure libft functions are declared

// Define t_shell before other headers
typedef struct s_shell {
    char **envp;
    int last_status;
} t_shell;

// Now include other headers that use t_shell
#include "parser.h"
#include "lexer.h"
#include "executor.h"
#include "builtins.h"
#include "signals.h"

// Global externs
extern volatile sig_atomic_t g_received_signal;

// Function prototypes
// utils.c
// char	*ft_strdup(const char *s1);
// size_t	ft_strlen(const char *s);
// char	*ft_strjoin(char const *s1, char const *s2);
// char    *ft_substr(char const *s, unsigned int start, size_t len);
// int     ft_strcmp(const char *s1, const char *s2);
// int     ft_strncmp(const char *s1, const char *s2, size_t n); // Add this
// int     ft_isalpha(int c);
// int     ft_isalnum(int c);
// char    *ft_itoa(int n);
// char    **ft_split(char const *s, char c);
void    *xmalloc(size_t size);
void    *xrealloc(void *ptr, size_t size);
void    xfree(void *ptr);
// char    *ft_strchr(const char *s, int c);
char    *ft_strjoin_free(char *s1, char *s2);
// char    *ft_strndup(const char *s, size_t n);
// char    *ft_strncpy(char *dst, const char *src, size_t n);
// void    ft_strcpy(char *dst, const char *src); // Add this if not in libft

// env.c
char    **env_copy(char **envp);
char    *env_get_value(char **envp, const char *key);
int     env_set_value(char ***envp, const char *key, const char *value);
void    env_remove_key(char ***envp, const char *key);
void    env_print(char **envp);

// expansions
char    *expand_variables(t_shell *shell, const char *str);

// main minishell loop
void    shell_loop(t_shell *shell);

// parser.c
t_command *parse_line(t_shell *shell, const char *line);
void free_command_list(t_command *cmd);

// helper functions
t_redir *redir_new(t_redir_type type, char *file);
void    redir_add_back(t_redir **lst, t_redir *new);

// lexer.c
t_token *lexer_tokenize(const char *line);
void free_token_list(t_token *tokens);

// executor.c
int execute_pipeline(t_shell *shell, t_command *cmd);

// builtins.c
int is_builtin(char *cmd);
int run_builtin(t_shell *shell, char **argv);

// builtins/*.c
int builtin_echo(char **argv);
int builtin_cd(t_shell *shell, char **argv);
int builtin_pwd(void);
int builtin_export(t_shell *shell, char **argv);
int builtin_unset(t_shell *shell, char **argv);
int builtin_env(t_shell *shell);
int builtin_exit(t_shell *shell, char **argv);

// signals.c
void setup_signals(void);
void handle_signal(int sig);

#endif
