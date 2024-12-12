#include "minishell.h"

static int is_valid_key(const char *k)
{
    // must not start with digit and only alnum and _
    if(!ft_isalpha(k[0]) && k[0]!='_')return 0;
    for(int i=1;k[i] && k[i]!='=';i++){
        if(!ft_isalnum(k[i])&&k[i]!='_')return 0;
    }
    return 1;
}

int builtin_export(t_shell *shell, char **argv)
{
    if(!argv[1])
    {
        // print env in format declare -x ...
        for(int i=0;shell->envp[i];i++){
            printf("declare -x %s\n",shell->envp[i]);
        }
        return 0;
    }
    for(int i=1;argv[i];i++){
        char *eq=ft_strchr(argv[i],'=');
        if(eq)
        {
            size_t klen=eq-argv[i];
            char *key=ft_substr(argv[i],0,klen);
            char *val=ft_strdup(eq+1);
            if(!is_valid_key(key))
            {
                fprintf(stderr,"minishell: export: `%s': not a valid identifier\n",argv[i]);
                xfree(key);xfree(val);
                continue;
            }
            env_set_value(&shell->envp,key,val);
            xfree(key);xfree(val);
        }
        else
        {
            // just add empty if key not exist
            if(is_valid_key(argv[i]))
                env_set_value(&shell->envp,argv[i],"");
            else
                fprintf(stderr,"minishell: export: `%s': not a valid identifier\n",argv[i]);
        }
    }
    return 0;
}
