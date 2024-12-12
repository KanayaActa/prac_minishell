#include "minishell.h"

int builtin_env(t_shell *shell)
{
    env_print(shell->envp);
    return 0;
}
