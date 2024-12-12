#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "minishell.h"

// The executor runs the pipeline of t_command
// Use pipes, fork, execve.
// handle builtins and redirections.
int execute_pipeline(t_shell *shell, t_command *cmd);
int open_redirs(t_command *c);

#endif
