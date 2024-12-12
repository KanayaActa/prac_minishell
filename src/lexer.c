#include "minishell.h"

static int is_metachar(int c)
{
    return (c=='|'||c=='<'||c=='>'||c=='\0');
}

static t_token *token_new(t_token_type type, char *val)
{
    t_token *t= xmalloc(sizeof(t_token));
    t->type=type; t->value=val; t->next=NULL;
    return t;
}

static void token_add_back(t_token **lst, t_token *new)
{
    if(!*lst){*lst=new;return;}
    t_token *p=*lst;
    while(p->next)p=p->next;
    p->next=new;
}

static t_token_type detect_type(const char *line, size_t *i)
{
    if(line[*i]=='|') {(*i)++;return T_PIPE;}
    else if(line[*i]=='<'){
        (*i)++;
        if(line[*i]=='<'){(*i)++;return T_HEREDOC;}
        return T_REDIR_IN;
    }
    else if(line[*i]=='>'){
        (*i)++;
        if(line[*i]=='>'){(*i)++;return T_APPEND;}
        return T_REDIR_OUT;
    }
    return T_END; //should not happen
}

static char *read_word(const char *line, size_t *i)
{
    size_t start=*i;
    int in_squote=0,in_dquote=0;
    while(line[*i]){
        if(!in_squote && !in_dquote && is_metachar(line[*i]))
            break;
        if(!in_squote && line[*i]=='"' && !in_dquote)
            in_dquote=1;
        else if(in_dquote && line[*i]=='"')
            in_dquote=0;
        else if(!in_dquote && line[*i]=='\'' && !in_squote)
            in_squote=1;
        else if(in_squote && line[*i]=='\'')
            in_squote=0;
        else if(!in_squote && !in_dquote && (line[*i]==' '||line[*i]=='\t'))
            break;
        (*i)++;
    }
    return ft_substr(line,start,*i - start);
}

t_token *lexer_tokenize(const char *line)
{
    t_token *tokens=NULL;
    size_t i=0;
    while(line[i])
    {
        while(line[i] && (line[i]==' '||line[i]=='\t'))
            i++;
        if(!line[i]) break;
        if (line[i]=='|'||line[i]=='<'||line[i]=='>')
        {
            t_token_type tt=detect_type(line,&i);
            token_add_back(&tokens, token_new(tt, ft_strdup("")));
        }
        else
        {
            char *w=read_word(line,&i);
            token_add_back(&tokens,token_new(T_WORD,w));
        }
    }
    return tokens;
}

void free_token_list(t_token *tokens)
{
    while(tokens)
    {
        t_token *n=tokens->next;
        xfree(tokens->value);
        xfree(tokens);
        tokens=n;
    }
}
