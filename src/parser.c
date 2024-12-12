#include "minishell.h"

t_redir *redir_new(t_redir_type type, char *file)
{
    t_redir *r=xmalloc(sizeof(t_redir));
    r->type=type;
    r->filename=file;
    r->next=NULL;
    return r;
}

void redir_add_back(t_redir **lst, t_redir *new)
{
    if(!*lst){*lst=new;return;}
    t_redir *p=*lst;
    while(p->next)p=p->next;
    p->next=new;
}

static t_command *cmd_new()
{
    t_command *c=xmalloc(sizeof(t_command));
    c->argv=NULL;
    c->redir=NULL;
    c->next=NULL;
    return c;
}

static void add_arg(t_command *cmd, char *arg)
{
    int count=0; 
    if(cmd->argv)
        while(cmd->argv[count])count++;
    cmd->argv=xrealloc(cmd->argv,(count+2)*sizeof(char*));
    cmd->argv[count]=arg;
    cmd->argv[count+1]=NULL;
}

static t_redir_type token_to_redir(t_token_type t)
{
    if(t==T_REDIR_IN)return R_INPUT;
    if(t==T_REDIR_OUT)return R_OUTPUT;
    if(t==T_APPEND)return R_APPEND;
    if(t==T_HEREDOC)return R_HEREDOC;
    return R_INPUT; // default
}

t_command *parse_line(t_shell *shell, const char *line)
{
    (void)shell; // expansions done later
    t_token *toks=lexer_tokenize(line);
    if(!toks)return NULL;

    t_command *cmds=NULL, *cur=NULL;
    t_token *p=toks;
    while(p)
    {
        if(!cur)
        {
            cur=cmd_new();
            // add to list
            cur->next=cmds;
            cmds=cur;
        }

        if(p->type==T_PIPE)
        {
            // next command
            t_command *newc=cmd_new();
            // append front because we can reverse later
            newc->next=cmds;
            cmds=newc;
            cur=newc;
            p=p->next;
            continue;
        }
        else if(p->type==T_WORD)
        {
            // expand variables here?
            char *expanded=expand_variables(shell,p->value);
            add_arg(cur, expanded);
        }
        else if(p->type==T_REDIR_IN||p->type==T_REDIR_OUT||p->type==T_APPEND||p->type==T_HEREDOC)
        {
            t_redir_type rt=token_to_redir(p->type);
            p=p->next;
            if(!p || p->type!=T_WORD){ // syntax error
                // free and return
                free_token_list(toks);
                free_command_list(cmds);
                return NULL;
            }
            char *expanded=expand_variables(shell,p->value);
            redir_add_back(&cur->redir, redir_new(rt,expanded));
        }
        p=p->next;
    }

    // Reverse the command list to restore correct order
    // Actually, we prepended on pipe
    // Let's reverse:
    t_command *prev=NULL, *nex=NULL;
    cur=cmds;
    while(cur){
        nex=cur->next;
        cur->next=prev;
        prev=cur;
        cur=nex;
    }
    cmds=prev;

    free_token_list(toks);
    return cmds;
}

void free_command_list(t_command *cmd)
{
    while(cmd)
    {
        t_command *n=cmd->next;
        if(cmd->argv)
        {
            for(int i=0;cmd->argv[i];i++)
                xfree(cmd->argv[i]);
            xfree(cmd->argv);
        }
        t_redir *r=cmd->redir;
        while(r)
        {
            t_redir *rn=r->next;
            xfree(r->filename);
            xfree(r);
            r=rn;
        }
        xfree(cmd);
        cmd=n;
    }
}
