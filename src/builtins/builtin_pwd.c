#include "minishell.h"

int builtin_pwd(void)
{
    char cwd[1024];
    if(!getcwd(cwd,1024)){
        fprintf(stderr,"minishell: pwd: error\n");
        return 1;
    }
    printf("%s\n",cwd);
    return 0;
}
