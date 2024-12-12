#include "minishell.h"

int builtin_echo(char **argv)
{
    int n_flag=0;
    int i=1;
    while(argv[i] && !ft_strcmp(argv[i],"-n")){
        n_flag=1;i++;
    }
    for(;argv[i];i++){
        printf("%s",argv[i]);
        if(argv[i+1])printf(" ");
    }
    if(!n_flag)printf("\n");
    return 0;
}
