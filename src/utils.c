#include "minishell.h"
#include <stdlib.h>

void *xmalloc(size_t size)
{
    void *p = malloc(size);
    if(!p){fprintf(stderr,"minishell: allocation error\n");exit(1);}
    return p;
}
void *xrealloc(void *ptr, size_t size)
{
    void *p = realloc(ptr,size);
    if(!p){fprintf(stderr,"minishell: allocation error\n");exit(1);}
    return p;
}
void xfree(void *ptr){ if(ptr) free(ptr); }

char *ft_strjoin_free(char *s1, char *s2)
{
    char *res=ft_strjoin(s1,s2);
    xfree(s1);
    xfree(s2);
    return res;
}

