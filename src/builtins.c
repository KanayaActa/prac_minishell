#include "minishell.h"

int is_builtin(char *cmd)
{
    if(!cmd)return 0;
    if(!ft_strcmp(cmd,"echo"))return 1;
    if(!ft_strcmp(cmd,"cd"))return 1;
    if(!ft_strcmp(cmd,"pwd"))return 1;
    if(!ft_strcmp(cmd,"export"))return 1;
    if(!ft_strcmp(cmd,"unset"))return 1;
    if(!ft_strcmp(cmd,"env"))return 1;
    if(!ft_strcmp(cmd,"exit"))return 1;
    return 0;
}

int run_builtin(t_shell *shell, char **argv)
{
    if(!ft_strcmp(argv[0],"echo"))return builtin_echo(argv);
    if(!ft_strcmp(argv[0],"cd"))return builtin_cd(shell,argv);
    if(!ft_strcmp(argv[0],"pwd"))return builtin_pwd();
    if(!ft_strcmp(argv[0],"export"))return builtin_export(shell,argv);
    if(!ft_strcmp(argv[0],"unset"))return builtin_unset(shell,argv);
    if(!ft_strcmp(argv[0],"env"))return builtin_env(shell);
    if(!ft_strcmp(argv[0],"exit"))return builtin_exit(shell,argv);
    return 127; // should not reach
}
