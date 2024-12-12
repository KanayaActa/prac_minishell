#include "minishell.h"

static char *expand_var(t_shell *shell, const char *var)
{
    if(!ft_strcmp(var,"?"))
    {
        return ft_itoa(shell->last_status);
    }
    char *val=env_get_value(shell->envp,var);
    if(!val) return ft_strdup("");
    return ft_strdup(val);
}

char *expand_variables(t_shell *shell, const char *str)
{
    // handle quotes and expansions
    // 'no expansions in single quotes'
    // expand $VAR and $?
    // skip expansions in single quotes, do in double quotes
    // minimal implementation
    char *res=ft_strdup("");
    int in_s=0,in_d=0;
    size_t i=0;
    while(str[i])
    {
        if(str[i]=='\'' && !in_d) { in_s=!in_s; i++; continue;}
        if(str[i]=='"' && !in_s) { in_d=!in_d; i++; continue;}
        if(str[i]=='$' && !in_s)
        {
            i++;
            if(str[i]=='?')
            {
                i++;
                char *e=expand_var(shell,"?");
                char *tmp=ft_strjoin_free(res,e);
                res=tmp;continue;
            }
            // parse var name
            size_t start=i;
            if (!((ft_isalpha(str[i])||str[i]=='_'))){ 
                // no valid var name, just append '$'
                char *tmp=ft_strjoin_free(res, ft_strndup("$",1));
                res=tmp; 
                continue;
            }
            while(str[i] && (ft_isalnum(str[i])||str[i]=='_'))i++;
            char *var=ft_strndup(str+start,i-start);
            char *val=expand_var(shell,var);
            xfree(var);
            char *tmp=ft_strjoin_free(res,val);
            res=tmp;
        }
        else
        {
            char *tmp=ft_strjoin_free(res, ft_strndup(&str[i],1));
            res=tmp;
            i++;
        }
    }
    return res;
}
