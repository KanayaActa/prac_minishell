#include "minishell.h"

int builtin_unset(t_shell *shell, char **argv)
{
    for(int i=1;argv[i];i++){
        env_remove_key(&shell->envp,argv[i]);
    }
    return 0;
}
