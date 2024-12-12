#ifndef BUILTINS_H
#define BUILTINS_H

#include "minishell.h"

int builtin_echo(char **argv);
int builtin_cd(t_shell *shell, char **argv);
int builtin_pwd(void);
int builtin_export(t_shell *shell, char **argv);
int builtin_unset(t_shell *shell, char **argv);
int builtin_env(t_shell *shell);
int builtin_exit(t_shell *shell, char **argv);

// helper to check if a command is builtin and execute it
int is_builtin(char *cmd);
int run_builtin(t_shell *shell, char **argv);

#endif
