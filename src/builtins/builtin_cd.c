#include "minishell.h"

int builtin_cd(t_shell *shell, char **argv)
{
    // no options
    // cd relative or absolute
    char *path;
    if(!argv[1]) {
        path=env_get_value(shell->envp,"HOME");
        if(!path){fprintf(stderr,"minishell: cd: HOME not set\n");return 1;}
    } else path=argv[1];
    if(chdir(path)<0)
    {
        fprintf(stderr,"minishell: cd: %s: %s\n", path, strerror(errno));
        return 1;
    }
    // Update PWD
    char cwd[1024];
    getcwd(cwd,1024);
    env_set_value(&shell->envp,"PWD",cwd);
    return 0;
}
