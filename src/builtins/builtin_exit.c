#include "minishell.h"

int builtin_exit(t_shell *shell, char **argv)
{
    (void)shell;
    // just exit
    // if has numeric arg use it
    int status=0;
    if(argv[1])
        status=atoi(argv[1]);
    // print message only if needed
    // The subject not specify we must print something
    exit(status);
    return 0; // never reach
}
