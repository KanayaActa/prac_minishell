#include "minishell.h"

char **env_copy(char **envp)
{
    int count=0;while(envp[count])count++;
    char **new=malloc((count+1)*sizeof(char*));
    for(int i=0;i<count;i++)
        new[i]=ft_strdup(envp[i]);
    new[count]=NULL;
    return new;
}

char *env_get_value(char **envp, const char *key)
{
    size_t klen=ft_strlen(key);
    for(int i=0;envp[i];i++){
        if(!ft_strncmp(envp[i],key,klen) && envp[i][klen]=='=')
            return envp[i]+klen+1;
    }
    return NULL;
}

int env_set_value(char ***envp, const char *key, const char *value)
{
    size_t klen=ft_strlen(key);
    int i; for(i=0;(*envp)[i];i++){
        if(!ft_strncmp((*envp)[i],key,klen) && (*envp)[i][klen]=='='){
            char *newval= xmalloc(klen+ft_strlen(value)+2);
            ft_strncpy(newval,key,klen);
            newval[klen]='=';
            ft_strcpy(newval+klen+1,value);
            free((*envp)[i]);
            (*envp)[i]=newval;
            return 0;
        }
    }
    // not found, append
    char **new = xrealloc(*envp,(i+2)*sizeof(char*));
    *envp=new;
    char *newval= xmalloc(klen+ft_strlen(value)+2);
    ft_strncpy(newval,key,klen);
    newval[klen]='=';
    ft_strcpy(newval+klen+1,value);
    (*envp)[i]=newval;
    (*envp)[i+1]=NULL;
    return 0;
}

void env_remove_key(char ***envp, const char *key)
{
    size_t klen=ft_strlen(key);
    int i,j;
    for(i=0;(*envp)[i];i++){
        if(!ft_strncmp((*envp)[i],key,klen) && (*envp)[i][klen]=='='){
            free((*envp)[i]);
            for(j=i;(*envp)[j];j++)
                (*envp)[j]=(*envp)[j+1];
            return;
        }
    }
}

void env_print(char **envp)
{
    for(int i=0; envp[i]; i++)
        printf("%s\n",envp[i]);
}
